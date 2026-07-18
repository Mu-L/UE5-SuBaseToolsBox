#include "Tools/IntelligentImportOfModelsAndMaterials/Import_MM.h"

// Unreal Engine 核心与 UI
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Images/SImage.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/TextureDefines.h"
// 资产操作相关
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "MaterialEditingLibrary.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"

// 系统与平台相关
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "DesktopPlatformModule.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxStaticMeshImportData.h"
#include "Interfaces/IMainFrameModule.h"
#include "Materials/Material.h"

#define LOCTEXT_NAMESPACE "SImport_MM"

SImport_MM::SImport_MM()
{
}

void SImport_MM::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(SVerticalBox)

        // --- 1. 引导区：说明文字 ---
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(15.0f, 10.0f)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight()
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ImportTitle", "智能模型与材质导入工具"))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 5)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ImportSteps", "操作流程：\n1. 选择根目录\n2. 系统自动识别每个子文件夹中的模型与贴图\n3. 自动匹配关键词、生成材质并完成模型绑定"))
                .AutoWrapText(true)
                .ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f))
            ]
        ]

        // --- 2. 教程区：占位图 ---
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(15.0f, 5.0f)
        [
            SNew(SBox)
            .WidthOverride(500.0f)
            .HeightOverride(150.0f)
            [
                SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("DarkSlateBorder"))
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(SImage)
                    .Image(FAppStyle::GetBrush("Icons.Documentation"))
                    .DesiredSizeOverride(FVector2D(48, 48))
                ]
            ]
        ]

        + SVerticalBox::Slot().AutoHeight().Padding(0, 10) [ SNew(SSeparator) ]

        // --- 3. 交互区：路径选择与按钮 ---
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(15.0f)
        [
            SNew(SHorizontalBox)
            
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            .VAlign(VAlign_Center)
            [
                SAssignNew(PathTextBox, SEditableTextBox)
                .HintText(LOCTEXT("PathHint", "请选择文件夹路径..."))
                .IsReadOnly(true)
            ]

            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(5, 0)
            [
                SNew(SButton)
                .Text(LOCTEXT("BrowseBtn", "浏览..."))
                .OnClicked(this, &SImport_MM::OnBrowseClicked)
            ]

            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "PrimaryButton")
                .ContentPadding(FMargin(20, 5))
                .OnClicked(this, &SImport_MM::OnStartImportClicked)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ExecuteBtn", "开始导入"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
                ]
            ]
        ]
    ];
}

FReply SImport_MM::OnBrowseClicked()
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (DesktopPlatform)
    {
        IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
        void* ParentHandle = MainFrameModule.GetParentWindow().IsValid() ? MainFrameModule.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle() : nullptr;
 
        FString FolderPath;
        if (DesktopPlatform->OpenDirectoryDialog(ParentHandle, TEXT("选择目录"), TEXT(""), FolderPath))
        {
            SelectedFolderPath = FolderPath;
            PathTextBox->SetText(FText::FromString(SelectedFolderPath));
        }
    }
    return FReply::Handled();
}

FReply SImport_MM::OnStartImportClicked()
{
    if (SelectedFolderPath.IsEmpty()) return FReply::Handled();
 
    IFileManager& FileManager = IFileManager::Get();
	
    // 辅助 Lambda 函数：扫描指定目录并执行导入
    auto TryProcessFolder = [&](const FString& InPath, const FString& FolderName)
    {
        FImportFolderTask Task;
        Task.FolderName = FolderName;
        TArray<FString> Files;
        FileManager.FindFiles(Files, *InPath, nullptr);
 
        for (const FString& FileName : Files)
        {
            FString Ext = FPaths::GetExtension(FileName).ToLower();
            FString AbsPath = InPath / FileName;
 
            if (Ext == TEXT("fbx")) Task.MeshPath = AbsPath;
            else if (Ext == TEXT("png") || Ext == TEXT("tga") || Ext == TEXT("jpg"))
            {
                FString Lower = FileName.ToLower();
                if (Lower.Contains(TEXT("base")) || Lower.Contains(TEXT("albedo")) || Lower.Contains(TEXT("col"))) Task.TextureMap.Add(TEXT("BaseColor"), AbsPath);
                else if (Lower.Contains(TEXT("normal")) || Lower.Contains(TEXT("nrm"))) Task.TextureMap.Add(TEXT("Normal"), AbsPath);
                else if (Lower.Contains(TEXT("rough"))) Task.TextureMap.Add(TEXT("Roughness"), AbsPath);
                else if (Lower.Contains(TEXT("metal"))) Task.TextureMap.Add(TEXT("Metallic"), AbsPath);
            }
        }
 
        if (!Task.MeshPath.IsEmpty())
        {
            ExecuteImportTask(Task);
            return true;
        }
        return false;
    };
 
    // 1. 先尝试导入当前选中的根目录 (处理“只想导一个文件夹”的情况)
    TryProcessFolder(SelectedFolderPath, FPaths::GetBaseFilename(SelectedFolderPath));
 
    // 2. 再尝试导入所有子文件夹 (处理批量导入)
    TArray<FString> SubFolders;
    FileManager.FindFiles(SubFolders, *(SelectedFolderPath / TEXT("*")), false, true);
 
    for (const FString& FolderName : SubFolders)
    {
        TryProcessFolder(SelectedFolderPath / FolderName, FolderName);
    }
 
    return FReply::Handled();
}

void SImport_MM::ExecuteImportTask(const FImportFolderTask& Task)
{
    IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FString TargetPath = TEXT("/Game/BatchImport/") + Task.FolderName;
 
    // --- 1. 导入模型：修复缩放和报错的关键设置 ---
    UFbxFactory* FbxFactory = NewObject<UFbxFactory>();
    FbxFactory->ImportUI->bIsObjImport = false;
    FbxFactory->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    FbxFactory->ImportUI->bImportMaterials = false;
    FbxFactory->ImportUI->bImportTextures = false;
 
    // 修复尺寸的核心：保持 1.0 缩放并强制转换场景单位
    FbxFactory->ImportUI->StaticMeshImportData->ImportUniformScale = 1.0f;
    FbxFactory->ImportUI->StaticMeshImportData->bConvertScene = true;      // 关键：将 DCC 坐标转为 UE 坐标
    FbxFactory->ImportUI->StaticMeshImportData->bRemoveDegenerates = false; // 消除退化报错
    FbxFactory->ImportUI->StaticMeshImportData->bCombineMeshes = false;
 
    TArray<FString> MeshFiles;
    MeshFiles.Add(Task.MeshPath);
    TArray<UObject*> ImportedMeshes = AssetTools.ImportAssets(MeshFiles, TargetPath, FbxFactory);
 
    // --- 2. 导入贴图 (分批导入以确保 Factory 识别正确) ---
    TArray<FString> TexFiles;
    for (auto& Pair : Task.TextureMap) TexFiles.Add(Pair.Value);
    TArray<UObject*> ImportedTexturesRaw = AssetTools.ImportAssets(TexFiles, TargetPath);
 
    // --- 3. 整理资产并自动连材质 (逻辑同前，保持稳定) ---
    TArray<UStaticMesh*> AllMeshes;
    for (UObject* Obj : ImportedMeshes) if (UStaticMesh* M = Cast<UStaticMesh>(Obj)) AllMeshes.Add(M);
 
    TMap<FString, UTexture2D*> ValidTextures;
    for (UObject* Obj : ImportedTexturesRaw)
    {
        if (UTexture2D* Tex = Cast<UTexture2D>(Obj))
        {
            FString Name = Tex->GetName().ToLower();
            if (Name.Contains(TEXT("normal")) || Name.Contains(TEXT("nrm")))
            {
                Tex->CompressionSettings = TextureCompressionSettings::TC_Normalmap;
                Tex->SRGB = false;
                Tex->PostEditChange();
                ValidTextures.Add(TEXT("Normal"), Tex);
            }
            else if (Name.Contains(TEXT("base")) || Name.Contains(TEXT("albedo"))) ValidTextures.Add(TEXT("BaseColor"), Tex);
            else if (Name.Contains(TEXT("rough"))) ValidTextures.Add(TEXT("Roughness"), Tex);
            else if (Name.Contains(TEXT("metal"))) ValidTextures.Add(TEXT("Metallic"), Tex);
        }
    }
 
    // --- 4. 材质赋予所有部件 ---
    if (AllMeshes.Num() > 0)
    {
        UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
        UMaterial* NewMat = Cast<UMaterial>(AssetTools.CreateAsset(TEXT("M_") + Task.FolderName, TargetPath, UMaterial::StaticClass(), MatFact));
 
        if (NewMat)
        {
            for (auto& Entry : ValidTextures)
            {
                auto* Sample = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                Sample->Texture = Entry.Value;
                EMaterialProperty Prop = EMaterialProperty::MP_BaseColor;
                if (Entry.Key == "Normal") { Prop = EMaterialProperty::MP_Normal; Sample->SamplerType = SAMPLERTYPE_Normal; }
                else if (Entry.Key == "Roughness") Prop = EMaterialProperty::MP_Roughness;
                else if (Entry.Key == "Metallic") Prop = EMaterialProperty::MP_Metallic;
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT(""), Prop);
            }
            UMaterialEditingLibrary::RecompileMaterial(NewMat);
            
            for (UStaticMesh* Mesh : AllMeshes)
            {
                Mesh->GetStaticMaterials().Empty();
                Mesh->GetStaticMaterials().Add(FStaticMaterial(NewMat));
                for (int32 i = 0; i < 16; ++i) { Mesh->SetMaterial(i, NewMat); }
                Mesh->PostEditChange();
            }
        }
    }
}
 

#undef LOCTEXT_NAMESPACE