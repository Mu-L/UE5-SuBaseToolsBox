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
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
 
    // --- 1. 获取 FBX 文件名前缀（用于剥离） ---
    FString FBXBaseName = FPaths::GetBaseFilename(Task.MeshPath);
    FString FBXPrefix = FBXBaseName + TEXT("_");
 
    // --- 2. 导入模型 ---
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
 
    // --- 3. 导入贴图并刷新 ---
    TArray<FString> AllTexPaths;
    Task.TextureMap.GenerateValueArray(AllTexPaths);
    if (AllTexPaths.Num() > 0)
    {
        AssetTools.ImportAssets(AllTexPaths, TargetPath, nullptr);
    }
    AssetRegistryModule.Get().ScanPathsSynchronous({TargetPath});
 
    // --- 4. 获取导入的模型资产 ---
    TArray<FAssetData> FoundMeshAssets;
    AssetRegistryModule.Get().GetAssetsByPath(FName(*TargetPath), FoundMeshAssets);
    TArray<UStaticMesh*> ImportedMeshes;
    for (const FAssetData& Ad : FoundMeshAssets) if (UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset())) ImportedMeshes.Add(M);
 
    if (ImportedMeshes.Num() == 0) return;
 
    // --- 5. 针对每个子模型处理材质与贴图匹配 ---
    for (UStaticMesh* MeshAsset : ImportedMeshes)
    {
        FString FullName = MeshAsset->GetName();
        FString CleanName = FullName.StartsWith(FBXPrefix) ? FullName.RightChop(FBXPrefix.Len()) : FullName;
 
        TMap<FString, UTexture2D*> PartTextures;
        for (auto& TexPair : Task.TextureMap)
        {
            FString TexFileName = FPaths::GetBaseFilename(TexPair.Value);
            if (TexFileName.Contains(CleanName, ESearchCase::IgnoreCase) || CleanName.Contains(TexFileName, ESearchCase::IgnoreCase))
            {
                FString AssetPath = TargetPath + TEXT("/") + TexFileName + TEXT(".") + TexFileName;
                if (UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *AssetPath))
                {
                    FString LowTex = TexFileName.ToLower();
                    // 分类逻辑：增加了更多关键字支持
                    if (LowTex.Contains(TEXT("base")) || LowTex.Contains(TEXT("albedo")) || LowTex.Contains(TEXT("col"))) 
                        PartTextures.Add(TEXT("Base"), Tex);
                    else if (LowTex.Contains(TEXT("normal")) || LowTex.Contains(TEXT("nrm"))) 
                        PartTextures.Add(TEXT("Normal"), Tex);
                    else if (LowTex.Contains(TEXT("rough")) || LowTex.Contains(TEXT("metal")) || LowTex.Contains(TEXT("occ")) || LowTex.Contains(TEXT("ao"))) 
                        PartTextures.Add(TEXT("ORM"), Tex);
                    else if (LowTex.Contains(TEXT("opacity")) || LowTex.Contains(TEXT("alpha")) || LowTex.Contains(TEXT("trans"))) 
                        PartTextures.Add(TEXT("Opacity"), Tex);
                    else if (LowTex.Contains(TEXT("specular")) || LowTex.Contains(TEXT("spec"))) 
                        PartTextures.Add(TEXT("Specular"), Tex);
                    else if (LowTex.Contains(TEXT("emissive")) || LowTex.Contains(TEXT("glow"))) 
                        PartTextures.Add(TEXT("Emissive"), Tex);
                    else if (LowTex.Contains(TEXT("wpo")) || LowTex.Contains(TEXT("offset"))) 
                        PartTextures.Add(TEXT("WPO"), Tex);
                }
            }
        }
 
        if (PartTextures.Num() == 0) continue;
 
        // 创建专属材质
        UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
        UMaterial* NewMat = Cast<UMaterial>(AssetTools.CreateAsset(TEXT("M_") + CleanName, TargetPath, UMaterial::StaticClass(), MatFact));
 
        if (NewMat)
        {
            // 特殊逻辑：如果检测到透明贴图，自动开启半透明模式
            if (PartTextures.Contains(TEXT("Opacity")))
            {
                NewMat->BlendMode = BLEND_Translucent;
                // 注意：在 C++ 中修改材质属性后通常需要更新编辑器
            }
 
            int32 YPos = 0;
            // 定义处理顺序
            TArray<FString> Categories = { TEXT("Base"), TEXT("Normal"), TEXT("ORM"), TEXT("Opacity"), TEXT("Specular"), TEXT("Emissive"), TEXT("WPO") };
 
            for (const FString& Cat : Categories)
            {
                if (!PartTextures.Contains(Cat)) continue;
                UTexture2D* T = PartTextures[Cat];
 
                // 确定压缩设置
                bool bNormal = (Cat == TEXT("Normal"));
                bool bMask = (Cat == TEXT("ORM") || Cat == TEXT("Opacity") || Cat == TEXT("Specular"));
                
                T->CompressionSettings = bNormal ? TC_Normalmap : (bMask ? TC_Masks : TC_Default);
                T->SRGB = !bNormal && !bMask;
                T->PostEditChange();
 
                auto* Node = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                Node->Texture = T;
                Node->MaterialExpressionEditorY = YPos; YPos += 300;
 
                // 设置采样器类型
                if (bNormal) Node->SamplerType = SAMPLERTYPE_Normal;
                else if (bMask) Node->SamplerType = SAMPLERTYPE_Masks;
 
                // 连线逻辑
                if (Cat == TEXT("Base")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), EMaterialProperty::MP_BaseColor);
                else if (Cat == TEXT("Normal")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), EMaterialProperty::MP_Normal);
                else if (Cat == TEXT("Opacity")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), EMaterialProperty::MP_Opacity);
                else if (Cat == TEXT("Specular")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), EMaterialProperty::MP_Specular);
                else if (Cat == TEXT("Emissive")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), EMaterialProperty::MP_EmissiveColor);
                else if (Cat == TEXT("WPO")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), EMaterialProperty::MP_WorldPositionOffset);
                else if (Cat == TEXT("ORM"))
                {
                    // 动态 ORM 通道排序连线 (保持之前的稳健逻辑)
                    struct FChan { int32 P; EMaterialProperty Prop; bool operator<(const FChan& O) const { return P < O.P; } };
                    TArray<FChan> Sorter;
                    FString TN = T->GetName().ToLower();
                    auto AddC = [&](TArray<FString> K, EMaterialProperty P) {
                        for(auto& k : K){ int32 i = TN.Find(k); if(i != INDEX_NONE){ Sorter.Add({i, P}); break; } }
                    };
                    AddC({TEXT("occlusion"), TEXT("ao"), TEXT("occ")}, EMaterialProperty::MP_AmbientOcclusion);
                    AddC({TEXT("roughness"), TEXT("rough")}, EMaterialProperty::MP_Roughness);
                    AddC({TEXT("metallic"), TEXT("metal")}, EMaterialProperty::MP_Metallic);
                    Sorter.Sort();
 
                    FString Pins[] = { TEXT("R"), TEXT("G"), TEXT("B") };
                    for (int32 i = 0; i < Sorter.Num() && i < 3; ++i)
                        UMaterialEditingLibrary::ConnectMaterialProperty(Node, Pins[i], Sorter[i].Prop);
                }
            }
            UMaterialEditingLibrary::RecompileMaterial(NewMat);
            // 绑定材质到所有槽位
            for (int32 i = 0; i < MeshAsset->GetStaticMaterials().Num(); ++i) MeshAsset->SetMaterial(i, NewMat);
            MeshAsset->PostEditChange();
        }
    }
}
 
#undef LOCTEXT_NAMESPACE