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
 
    // --- 2. 模型导入 (含自适应缩放与原点对齐) ---
    UFbxFactory* FbxFact = NewObject<UFbxFactory>();
    FbxFact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    FbxFact->ImportUI->bImportMaterials = false;
    FbxFact->ImportUI->bImportTextures = false;
    FbxFact->ImportUI->StaticMeshImportData->bTransformVertexToAbsolute = true;
    FbxFact->ImportUI->StaticMeshImportData->bConvertSceneUnit = true;
    FbxFact->ImportUI->StaticMeshImportData->ImportUniformScale = 1.0f;
 
    UAssetImportTask* MeshTask = NewObject<UAssetImportTask>();
    MeshTask->Filename = Task.MeshPath;
    MeshTask->DestinationPath = TargetPath;
    MeshTask->Factory = FbxFact;
    MeshTask->bAutomated = true; // 消除弹窗
 
    AssetTools.ImportAssetTasks({ MeshTask });
 
    // 同步扫描
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    AssetRegistryModule.Get().ScanPathsSynchronous({TargetPath});
 
    // 获取所有导入的模型 (解决多物体导入问题)
    TArray<FAssetData> FoundMeshAssets;
    AssetRegistryModule.Get().GetAssetsByPath(FName(*TargetPath), FoundMeshAssets);
    TArray<UStaticMesh*> ImportedMeshes;
    for (const FAssetData& Ad : FoundMeshAssets) if (UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset())) ImportedMeshes.Add(M);
 
    if (ImportedMeshes.Num() == 0) return;
 
    // 缩放校准
    if (ImportedMeshes[0]->GetBounds().GetBox().GetSize().GetMax() < 2.0f) {
        FbxFact->ImportUI->StaticMeshImportData->ImportUniformScale = 100.0f;
        AssetTools.ImportAssetTasks({ MeshTask });
        AssetRegistryModule.Get().ScanPathsSynchronous({TargetPath});
    }
 
    // --- 3. 贴图导入 ---
    TArray<FString> TextureFiles;
    ValidTexturePaths.GenerateValueArray(TextureFiles);
    if (TextureFiles.Num() > 0) {
        AssetTools.ImportAssets(TextureFiles, TargetPath, nullptr);
        AssetRegistryModule.Get().ScanPathsSynchronous({TargetPath});
    }
 
    // --- 4. 智能材质生成与自适应通道连接 ---
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
            FString TexAssetName = FPaths::GetBaseFilename(DiskPath);
            FString FullTexPath = TargetPath + TEXT("/") + TexAssetName + TEXT(".") + TexAssetName;
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *FullTexPath);
            if (!Tex) continue;
 
            // 设置贴图压缩与SRGB
            bool bIsNormal = (Category == TEXT("Normal"));
            bool bIsORM = (Category == TEXT("ORM"));
            Tex->CompressionSettings = bIsNormal ? TC_Normalmap : (bIsORM ? TC_Masks : TC_Default);
            Tex->SRGB = !bIsNormal && !bIsORM;
            Tex->PostEditChange();
 
            // 创建材质表达式
            auto* Sample = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
            Sample->Texture = Tex;
            Sample->MaterialExpressionEditorY = YOffset; YOffset += 300;
            if (bIsNormal) Sample->SamplerType = SAMPLERTYPE_Normal;
            else if (bIsORM) Sample->SamplerType = SAMPLERTYPE_Masks;
 
            // --- 核心修复：自适应通道连线逻辑 ---
            if (Category == TEXT("Base")) {
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT(""), EMaterialProperty::MP_BaseColor);
            }
            else if (Category == TEXT("Normal")) {
                UMaterialEditingLibrary::ConnectMaterialProperty(Sample, TEXT(""), EMaterialProperty::MP_Normal);
            }
            else if (Category == TEXT("ORM")) {
                // 定义结构体用于排序
                struct FChannelMapping {
                    int32 Index;
                    EMaterialProperty Property;
                    bool operator<(const FChannelMapping& Other) const { return Index < Other.Index; }
                };
 
                TArray<FChannelMapping> FoundMappings;
                FString LowName = TexAssetName.ToLower();
 
                // 查找关键字在文件名中的位置
                auto CheckKeyword = [&](TArray<FString> Keywords, EMaterialProperty TargetProp) {
                    for (const FString& K : Keywords) {
                        int32 FoundPos = LowName.Find(K);
                        if (FoundPos != INDEX_NONE) {
                            FoundMappings.Add({FoundPos, TargetProp});
                            break;
                        }
                    }
                };
 
                // 注入识别关键字
                CheckKeyword({TEXT("occlusion"), TEXT("ao"), TEXT("occ")}, EMaterialProperty::MP_AmbientOcclusion);
                CheckKeyword({TEXT("roughness"), TEXT("rough")}, EMaterialProperty::MP_Roughness);
                CheckKeyword({TEXT("metallic"), TEXT("metal"), TEXT("met")}, EMaterialProperty::MP_Metallic);
 
                // 根据在文件名中出现的先后顺序排序 (例如 O-R-M 顺序)
                FoundMappings.Sort();
 
                // 按照排序后的顺序分配 R, G, B 通道
                FString Channels[] = { TEXT("R"), TEXT("G"), TEXT("B") };
                for (int32 i = 0; i < FoundMappings.Num() && i < 3; ++i) {
                    UMaterialEditingLibrary::ConnectMaterialProperty(Sample, Channels[i], FoundMappings[i].Property);
                }
            }
        }
        UMaterialEditingLibrary::RecompileMaterial(NewMat);
 
        // 赋予所有子物体材质
        for (UStaticMesh* Mesh : ImportedMeshes) {
            for (int32 i = 0; i < Mesh->GetStaticMaterials().Num(); ++i) Mesh->SetMaterial(i, NewMat);
            Mesh->PostEditChange();
        }
    }
}
 
#undef LOCTEXT_NAMESPACE