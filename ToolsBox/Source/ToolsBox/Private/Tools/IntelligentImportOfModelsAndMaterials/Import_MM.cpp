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
 
    // --- 1. 识别并归类贴图 (统一变量名: ValidTexturePaths) ---
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
 
    // --- 2. FBX 导入 (使用 Task 屏蔽 UI) ---
    UFbxFactory* FbxFact = NewObject<UFbxFactory>();
    FbxFact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    FbxFact->ImportUI->bImportMaterials = false;
    FbxFact->ImportUI->bImportTextures = false;
    FbxFact->ImportUI->StaticMeshImportData->bConvertScene = true;
 
    UAssetImportTask* MeshTask = NewObject<UAssetImportTask>();
    MeshTask->Filename = Task.MeshPath;
    MeshTask->DestinationPath = TargetPath;
    MeshTask->Factory = FbxFact;
    
    // 关键修复：直接使用 bAutomated。当其为 true 时，UE5 会自动消除除第一个模型外的弹窗。
    // 如果你的 UE5 版本识别不到 bShowImportOptions，只写这一个变量就够了。
    MeshTask->bAutomated = !bFirst; 
 
    AssetTools.ImportAssetTasks({ MeshTask });
 
    // --- 3. 贴图导入 ---
    TArray<FString> TextureFiles;
    ValidTexturePaths.GenerateValueArray(TextureFiles);
    if (TextureFiles.Num() > 0)
    {
        AssetTools.ImportAssets(TextureFiles, TargetPath, nullptr);
    }
 
    // --- 4. 生成材质逻辑 ---
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
            TArray<FString> Categories = { TEXT("Base"), TEXT("Normal"), TEXT("ORM") };
            
            for (const FString& Category : Categories)
            {
                if (!ValidTexturePaths.Contains(Category)) continue;
 
                FString DiskPath = ValidTexturePaths[Category];
                FString AssetName = FPaths::GetBaseFilename(DiskPath);
                FString FullObjectPath = TargetPath + TEXT("/") + AssetName + TEXT(".") + AssetName;
                
                UTexture2D* Tex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *FullObjectPath));
                if (!Tex) continue;
 
                TextureCompressionSettings Comp = TextureCompressionSettings::TC_Default;
                struct FConn { FString Pin; EMaterialProperty Prop; };
                TArray<FConn> Intents;
 
                if (Category == TEXT("Base")) { Intents.Add({ TEXT(""), EMaterialProperty::MP_BaseColor }); }
                else if (Category == TEXT("Normal")) { Comp = TextureCompressionSettings::TC_Normalmap; Intents.Add({ TEXT(""), EMaterialProperty::MP_Normal }); }
                else if (Category == TEXT("ORM"))
                {
                    Comp = TextureCompressionSettings::TC_Masks;
                    FString LowName = AssetName.ToLower();
                    if (LowName.Find(TEXT("rough")) != INDEX_NONE) Intents.Add({ TEXT("R"), EMaterialProperty::MP_Roughness });
                    if (LowName.Find(TEXT("metal")) != INDEX_NONE) Intents.Add({ TEXT("G"), EMaterialProperty::MP_Metallic });
                    if (LowName.Find(TEXT("ao")) != INDEX_NONE || LowName.Find(TEXT("occ")) != INDEX_NONE) Intents.Add({ TEXT("B"), EMaterialProperty::MP_AmbientOcclusion });
                }
 
                if (Intents.Num() > 0)
                {
                    Tex->CompressionSettings = Comp;
                    Tex->SRGB = (Comp == TextureCompressionSettings::TC_Normalmap || Comp == TextureCompressionSettings::TC_Masks) ? false : true;
                    Tex->PostEditChange();
 
                    auto* Sample = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                    Sample->Texture = Tex;
                    Sample->MaterialExpressionEditorY = YOffset; YOffset += 280;
 
                    if (Comp == TextureCompressionSettings::TC_Normalmap) Sample->SamplerType = SAMPLERTYPE_Normal;
                    else if (Comp == TextureCompressionSettings::TC_Masks) Sample->SamplerType = SAMPLERTYPE_Masks;
 
                    for (auto& Int : Intents) UMaterialEditingLibrary::ConnectMaterialProperty(Sample, Int.Pin, Int.Prop);
                }
            }
            UMaterialEditingLibrary::RecompileMaterial(NewMat);
            for (UStaticMesh* Mesh : ImportedMeshes) { for (int32 i = 0; i < 8; ++i) Mesh->SetMaterial(i, NewMat); Mesh->PostEditChange(); }
        }
    }
}
 
#undef LOCTEXT_NAMESPACE