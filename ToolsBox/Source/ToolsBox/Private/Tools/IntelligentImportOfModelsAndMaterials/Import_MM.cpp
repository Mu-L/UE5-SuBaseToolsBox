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
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 5)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ImportSteps", "操作流程：\n1. 选择根目录\n2. 系统自动识别每个子文件夹中的模型与贴图\n3. 自动匹配关键词、生成材质并完成模型绑定"))
                .AutoWrapText(true)
                .ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f))
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 10)
           [
               SNew(STextBlock)
               .Text(LOCTEXT("ImportSteps", ""))
               .AutoWrapText(true)
               .ColorAndOpacity(FLinearColor::Red)
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
        if (DesktopPlatform->OpenDirectoryDialog(ParentHandle, TEXT("选择导入目录"), TEXT(""), FolderPath))
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
 
    // 定义合法的贴图关键词白名单
    TArray<FString> ValidKeywords = { 
        TEXT("base"), TEXT("albedo"), TEXT("col"),      // BaseColor
        TEXT("normal"), TEXT("nrm"),                   // Normal
        TEXT("rough"), TEXT("metal"), TEXT("met"),     // ORM/PBR
        TEXT("occ"), TEXT("ao")                        // Ambient Occlusion
    };
 
    auto ScanAndRun = [&](const FString& Path, const FString& Name) {
        FImportFolderTask Task;
        Task.FolderName = Name;
        TArray<FString> Files;
        FileManager.FindFiles(Files, *Path, nullptr);
 
        for (const FString& F : Files)
        {
            FString E = FPaths::GetExtension(F).ToLower();
            FString LowName = F.ToLower();
 
            // 1. 识别 FBX
            if (E == TEXT("fbx")) 
            {
                Task.MeshPath = Path / F;
            }
            // 2. 识别贴图（核心：只有包含白名单关键词的才被加入 Task.TextureMap）
            else if (E == TEXT("png") || E == TEXT("tga") || E == TEXT("jpg") || E == TEXT("tif"))
            {
                bool bIsValidTexture = false;
                for (const FString& Key : ValidKeywords)
                {
                    if (LowName.Contains(Key))
                    {
                        bIsValidTexture = true;
                        break;
                    }
                }
 
                if (bIsValidTexture)
                {
                    // 只有符合条件的贴图路径才会存入 Task，不符合的直接“取消资格”
                    Task.TextureMap.Add(F, Path / F);
                }
            }
        }
 
        if (!Task.MeshPath.IsEmpty()) 
        {
            ExecuteImportTask(Task);
        }
    };
 
    // 执行根目录和子目录扫描
    ScanAndRun(SelectedFolderPath, FPaths::GetBaseFilename(SelectedFolderPath));
    TArray<FString> Subs;
    FileManager.FindFiles(Subs, *(SelectedFolderPath / TEXT("*")), false, true);
    for (const FString& S : Subs) ScanAndRun(SelectedFolderPath / S, S);
 
    return FReply::Handled();
}
 
void SImport_MM::ExecuteImportTask(const FImportFolderTask& Task)
{
    IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FString TargetPath = TEXT("/Game/BatchImport/") + Task.FolderName;
 
    // --- 1. 建立精准的【功能 -> 路径】映射，不符合要求的直接不进这个 Map ---
    TMap<FString, FString> ValidTexturePaths; // Key: 功能(Base/Normal/ORM), Value: 磁盘绝对路径
    
    for (auto& Pair : Task.TextureMap) // Task.TextureMap 是你在扫描阶段初步筛选的图片
    {
        FString FileName = Pair.Key.ToLower();
        if (FileName.Contains(TEXT("base")) || FileName.Contains(TEXT("albedo")) || FileName.Contains(TEXT("col")))
            ValidTexturePaths.Add(TEXT("Base"), Pair.Value);
        else if (FileName.Contains(TEXT("normal")) || FileName.Contains(TEXT("nrm")))
            ValidTexturePaths.Add(TEXT("Normal"), Pair.Value);
        else if (FileName.Contains(TEXT("rough")) || FileName.Contains(TEXT("metal")) || FileName.Contains(TEXT("occ")) || FileName.Contains(TEXT("ao")))
            ValidTexturePaths.Add(TEXT("ORM"), Pair.Value);
    }
 
    // --- 2. 导入模型 ---
    UFbxFactory* FbxFact = NewObject<UFbxFactory>();
    FbxFact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    FbxFact->ImportUI->bImportMaterials = false;
    FbxFact->ImportUI->bImportTextures = false;
    FbxFact->ImportUI->StaticMeshImportData->bConvertScene = true;
    
    AssetTools.ImportAssets({Task.MeshPath}, TargetPath, FbxFact);
 
    // --- 3. 导入选中的贴图 ---
    TArray<FString> FinalPathsToImport;
    ValidTexturePaths.GenerateValueArray(FinalPathsToImport);
    AssetTools.ImportAssets(FinalPathsToImport, TargetPath, nullptr);
 
    // --- 4. 生成材质逻辑（严格基于功能映射，不再遍历导入结果数组） ---
    // 先获取导入后的模型（为了后续赋材质）
    TArray<UStaticMesh*> ImportedMeshes;
    TArray<FAssetData> MeshAssets;
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    AssetRegistryModule.Get().GetAssetsByPath(FName(*TargetPath), MeshAssets);
    for (auto& Asset : MeshAssets) if (UStaticMesh* M = Cast<UStaticMesh>(Asset.GetAsset())) ImportedMeshes.Add(M);
 
    if (ImportedMeshes.Num() > 0)
    {
        UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
        UMaterial* NewMat = Cast<UMaterial>(AssetTools.CreateAsset(TEXT("M_") + Task.FolderName, TargetPath, UMaterial::StaticClass(), MatFact));
 
        if (NewMat)
        {
            int32 YOffset = 0;
            
            // 重点：遍历我们自己定义的“三类”功能关键词，而不是遍历资产数组
            TArray<FString> Categories = { TEXT("Base"), TEXT("Normal"), TEXT("ORM") };
            
            for (const FString& Category : Categories)
            {
                if (!ValidTexturePaths.Contains(Category)) continue;
 
                // 根据文件名获取导入后的 Texture 对象
                FString DiskPath = ValidTexturePaths[Category];
                FString AssetName = FPaths::GetBaseFilename(DiskPath);
                FString FullObjectPath = TargetPath + TEXT("/") + AssetName + TEXT(".") + AssetName;
                
                UTexture2D* Tex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *FullObjectPath));
                if (!Tex) continue;
 
                // --- 确定连线意图 ---
                struct FConn { FString Pin; EMaterialProperty Prop; };
                TArray<FConn> Intents;
                TextureCompressionSettings Comp = TextureCompressionSettings::TC_Default;
 
                if (Category == TEXT("Base"))
                {
                    Intents.Add({ TEXT(""), EMaterialProperty::MP_BaseColor });
                }
                else if (Category == TEXT("Normal"))
                {
                    Comp = TextureCompressionSettings::TC_Normalmap;
                    Intents.Add({ TEXT(""), EMaterialProperty::MP_Normal });
                }
                else if (Category == TEXT("ORM"))
                {
                    // ORM 通道排序逻辑
                    FString LowName = AssetName.ToLower();
                    TArray<FChannelMapping> Found;
                    auto Check = [&](FString K, EMaterialProperty P) {
                        int32 Idx = LowName.Find(K);
                        if (Idx != INDEX_NONE) Found.Add({ K, Idx, P });
                    };
                    Check(TEXT("rough"), EMaterialProperty::MP_Roughness);
                    Check(TEXT("metal"), EMaterialProperty::MP_Metallic);
                    Check(TEXT("occ"), EMaterialProperty::MP_AmbientOcclusion);
                    Check(TEXT("ao"), EMaterialProperty::MP_AmbientOcclusion);
                    
                    Found.Sort();
                    Comp = TextureCompressionSettings::TC_Masks;
                    FString RGB[] = { TEXT("R"), TEXT("G"), TEXT("B") };
                    for (int32 i = 0; i < Found.Num() && i < 3; ++i) Intents.Add({ RGB[i], Found[i].TargetProp });
                }
 
                // --- 此时才真正创建节点：保证一个 Category 只创建一个节点 ---
                if (Intents.Num() > 0)
                {
                    // 应用贴图设置
                    Tex->CompressionSettings = Comp;
                    Tex->SRGB = (Comp == TextureCompressionSettings::TC_Normalmap || Comp == TextureCompressionSettings::TC_Masks) ? false : true;
                    Tex->PostEditChange();
 
                    auto* Sample = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                    Sample->Texture = Tex;
                    Sample->MaterialExpressionEditorX = -600;
                    Sample->MaterialExpressionEditorY = YOffset;
                    YOffset += 280;
 
                    if (Comp == TextureCompressionSettings::TC_Normalmap) Sample->SamplerType = SAMPLERTYPE_Normal;
                    else if (Comp == TextureCompressionSettings::TC_Masks) Sample->SamplerType = SAMPLERTYPE_Masks;
 
                    for (auto& Int : Intents) UMaterialEditingLibrary::ConnectMaterialProperty(Sample, Int.Pin, Int.Prop);
                }
            }
 
            UMaterialEditingLibrary::RecompileMaterial(NewMat);
 
            for (UStaticMesh* Mesh : ImportedMeshes)
            {
                for (int32 i = 0; i < 16; ++i) Mesh->SetMaterial(i, NewMat);
                Mesh->PostEditChange();
            }
        }
    }
}
 
#undef LOCTEXT_NAMESPACE