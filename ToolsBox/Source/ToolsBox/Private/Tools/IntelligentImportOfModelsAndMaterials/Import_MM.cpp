#include "Tools/IntelligentImportOfModelsAndMaterials/Import_MM.h"
 
// Unreal Engine 核心与 UI
#include "Engine/TextureDefines.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"
 
// 核心资产与导入
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "MaterialEditingLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Factories/Factory.h"
#include "Factories/MaterialFactoryNew.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxStaticMeshImportData.h"
 
// 系统与平台
#include "AssetImportTask.h"
#include "DesktopPlatformModule.h"
#include "HAL/FileManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "Materials/Material.h"
#include "Misc/Paths.h"
#include "UnrealEdGlobals.h" // 包含 GIsAutomatedImport
 
#define LOCTEXT_NAMESPACE "SImport_MM"
 
SImport_MM::SImport_MM() {}
 
void SImport_MM::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(15.0f, 10.0f)
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
                .Text(LOCTEXT("ImportSteps", "操作流程：\n1. 选择根目录\n2. 第一个模型会弹出 FBX 选项，点击导入即可\n3. 后续模型将由系统强制静默导入。"))
                .AutoWrapText(true)
                .ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f))
            ]
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(15.0f)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().FillWidth(1.0f).VAlign(VAlign_Center)
            [
                SAssignNew(PathTextBox, SEditableTextBox).HintText(LOCTEXT("PathHint", "请选择文件夹路径...")).IsReadOnly(true)
            ]
            + SHorizontalBox::Slot().AutoWidth().Padding(5, 0)
            [
                SNew(SButton).Text(LOCTEXT("BrowseBtn", "浏览...")).OnClicked(this, &SImport_MM::OnBrowseClicked)
            ]
            + SHorizontalBox::Slot().AutoWidth()
            [
                SNew(SButton).ButtonStyle(FAppStyle::Get(), "PrimaryButton")
                .OnClicked(this, &SImport_MM::OnStartImportClicked)
                [
                    SNew(STextBlock).Text(LOCTEXT("ExecuteBtn", "开始导入"))
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
    TArray<FString> ValidKeywords = { TEXT("base"), TEXT("albedo"), TEXT("col"), TEXT("normal"), TEXT("nrm"), TEXT("rough"), TEXT("metal"), TEXT("met"), TEXT("occ"), TEXT("ao") };
 
    // 关键：第一个 FBX 弹出 UI，其余强制屏蔽
    bool bIsFirstImportSession = true;
 
    auto ScanAndRun = [&](const FString& Path, const FString& Name) {
        FImportFolderTask Task;
        Task.FolderName = Name;
        TArray<FString> Files;
        FileManager.FindFiles(Files, *Path, nullptr);
 
        for (const FString& F : Files)
        {
            FString E = FPaths::GetExtension(F).ToLower();
            if (E == TEXT("fbx")) Task.MeshPath = Path / F;
            else if (E == TEXT("png") || E == TEXT("tga") || E == TEXT("jpg") || E == TEXT("tif"))
            {
                for (const FString& Key : ValidKeywords) { if (F.ToLower().Contains(Key)) { Task.TextureMap.Add(F, Path / F); break; } }
            }
        }
 
        if (!Task.MeshPath.IsEmpty()) 
        {
            ExecuteImportTask(Task, bIsFirstImportSession);
            bIsFirstImportSession = false; // 只有第一次任务后设为 false
        }
    };
 
    ScanAndRun(SelectedFolderPath, FPaths::GetBaseFilename(SelectedFolderPath));
    TArray<FString> Subs;
    FileManager.FindFiles(Subs, *(SelectedFolderPath / TEXT("*")), false, true);
    for (const FString& S : Subs) ScanAndRun(SelectedFolderPath / S, S);
 
    return FReply::Handled();
}
 
void SImport_MM::ExecuteImportTask(const FImportFolderTask& Task, bool bFirst)
{
    IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FString TargetPath = TEXT("/Game/BatchImport/") + Task.FolderName;
 
    // --- 1. 贴图分类 (逻辑保持) ---
    TMap<FString, FString> ValidTexturePaths; 
    for (auto& Pair : Task.TextureMap)
    {
        FString FileName = Pair.Key.ToLower();
        if (FileName.Contains(TEXT("base")) || FileName.Contains(TEXT("albedo")) || FileName.Contains(TEXT("col")))
            ValidTexturePaths.Add(TEXT("Base"), Pair.Value);
        else if (FileName.Contains(TEXT("normal")) || FileName.Contains(TEXT("nrm")))
            ValidTexturePaths.Add(TEXT("Normal"), Pair.Value);
        else if (FileName.Contains(TEXT("rough")) || FileName.Contains(TEXT("metal")) || FileName.Contains(TEXT("occ")) || FileName.Contains(TEXT("ao")))
            ValidTexturePaths.Add(TEXT("ORM"), Pair.Value);
    }
 
    // --- 2. 配置 FBX 导入 (增强自适应与稳定性) ---
    UFbxFactory* FbxFact = NewObject<UFbxFactory>();
    FbxFact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    FbxFact->ImportUI->bImportMaterials = false;
    FbxFact->ImportUI->bImportTextures = false;
    
    // 关键配置
    FbxFact->ImportUI->StaticMeshImportData->bTransformVertexToAbsolute = true;
    FbxFact->ImportUI->StaticMeshImportData->bConvertSceneUnit = true;
    FbxFact->ImportUI->StaticMeshImportData->ImportUniformScale = 1.0f;
    
    // 如果你希望多个子模型合并为一个资产，取消下面这行的注释：
    // FbxFact->ImportUI->StaticMeshImportData->bCombineMeshes = true;
 
    UAssetImportTask* MeshTask = NewObject<UAssetImportTask>();
    MeshTask->Filename = Task.MeshPath;
    MeshTask->DestinationPath = TargetPath;
    MeshTask->Factory = FbxFact;
    MeshTask->bAutomated = true; // 消除弹窗
 
    AssetTools.ImportAssetTasks({ MeshTask });
 
    // --- 3. 核心修复：扫描并获取文件夹内所有的模型 ---
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FString> PathsToScan; PathsToScan.Add(TargetPath);
    AssetRegistryModule.Get().ScanPathsSynchronous(PathsToScan);
 
    TArray<FAssetData> FoundMeshAssets;
    // 获取当前文件夹下所有的静态网格体
    AssetRegistryModule.Get().GetAssetsByPath(FName(*TargetPath), FoundMeshAssets);
 
    TArray<UStaticMesh*> ImportedMeshes;
    for (const FAssetData& AssetData : FoundMeshAssets)
    {
        if (UStaticMesh* Mesh = Cast<UStaticMesh>(AssetData.GetAsset()))
        {
            ImportedMeshes.Add(Mesh);
        }
    }
 
    if (ImportedMeshes.Num() == 0) return; // 没模型就直接退出
 
    // --- 4. 检查第一个模型的缩放 (自适应校准) ---
    UStaticMesh* FirstMesh = ImportedMeshes[0];
    float MaxDim = FirstMesh->GetBounds().GetBox().GetSize().GetMax();
    if (MaxDim > 0.0f && MaxDim < 2.0f)
    {
        FbxFact->ImportUI->StaticMeshImportData->ImportUniformScale = 100.0f;
        AssetTools.ImportAssetTasks({ MeshTask }); // 重新导入
        AssetRegistryModule.Get().ScanPathsSynchronous(PathsToScan);
        // 重新获取资产
        ImportedMeshes.Empty();
        AssetRegistryModule.Get().GetAssetsByPath(FName(*TargetPath), FoundMeshAssets);
        for (const FAssetData& Ad : FoundMeshAssets) if (UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset())) ImportedMeshes.Add(M);
    }
 
    // --- 5. 导入贴图 ---
    TArray<FString> TextureFiles;
    ValidTexturePaths.GenerateValueArray(TextureFiles);
    if (TextureFiles.Num() > 0)
    {
        AssetTools.ImportAssets(TextureFiles, TargetPath, nullptr);
        AssetRegistryModule.Get().ScanPathsSynchronous(PathsToScan);
    }
 
    // --- 6. 创建材质并绑定到所有找到的模型上 ---
    UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
    FString MatName = TEXT("M_") + Task.FolderName;
    UMaterial* NewMat = Cast<UMaterial>(AssetTools.CreateAsset(MatName, TargetPath, UMaterial::StaticClass(), MatFact));
 
    if (NewMat)
    {
        int32 YOffset = 0;
        TArray<FString> Categories = { TEXT("Base"), TEXT("Normal"), TEXT("ORM") };
        for (const FString& Category : Categories)
        {
            if (!ValidTexturePaths.Contains(Category)) continue;
 
            FString DiskPath = ValidTexturePaths[Category];
            FString TexName = FPaths::GetBaseFilename(DiskPath);
            FString FullTexPath = TargetPath + TEXT("/") + TexName + TEXT(".") + TexName;
            
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *FullTexPath);
            if (!Tex) continue;
 
            // 贴图设置与连线逻辑
            TextureCompressionSettings Comp = TextureCompressionSettings::TC_Default;
            if (Category == TEXT("Normal")) Comp = TextureCompressionSettings::TC_Normalmap;
            else if (Category == TEXT("ORM")) Comp = TextureCompressionSettings::TC_Masks;
 
            Tex->CompressionSettings = Comp;
            Tex->SRGB = (Comp == TextureCompressionSettings::TC_Default);
            Tex->PostEditChange();
 
            auto* Sample = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
            Sample->Texture = Tex;
            Sample->MaterialExpressionEditorY = YOffset; YOffset += 280;
 
            if (Category == TEXT("Base")) UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT(""), EMaterialProperty::MP_BaseColor);
            else if (Category == TEXT("Normal")) {
                Sample->SamplerType = SAMPLERTYPE_Normal;
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT(""), EMaterialProperty::MP_Normal);
            }
            else if (Category == TEXT("ORM")) {
                Sample->SamplerType = SAMPLERTYPE_Masks;
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT("R"), EMaterialProperty::MP_Roughness);
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT("G"), EMaterialProperty::MP_Metallic);
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT("B"), EMaterialProperty::MP_AmbientOcclusion);
            }
        }
        UMaterialEditingLibrary::RecompileMaterial(NewMat);
 
        // --- 核心修复：为所有子物体赋予材质 ---
        for (UStaticMesh* Mesh : ImportedMeshes)
        {
            int32 NumSlots = Mesh->GetStaticMaterials().Num();
            for (int32 i = 0; i < NumSlots; ++i)
            {
                Mesh->SetMaterial(i, NewMat);
            }
            Mesh->PostEditChange();
        }
    }
}
 
#undef LOCTEXT_NAMESPACE