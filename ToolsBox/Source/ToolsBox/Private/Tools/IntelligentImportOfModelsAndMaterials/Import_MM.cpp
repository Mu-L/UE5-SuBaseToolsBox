
#include "Tools/IntelligentImportOfModelsAndMaterials/Import_MM.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetImportTask.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxStaticMeshImportData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MaterialEditingLibrary.h"
#include "Factories/MaterialFactoryNew.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/PackageName.h"
#include "DesktopPlatformModule.h"
#include "Framework/Text/ITextDecorator.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Interfaces/IMainFrameModule.h"
#include "Materials/Material.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SImport_MM"

void SImport_MM::Construct(const FArguments& InArgs)
{
     // Initialize a Marshaller that allows inline color formatting
    TArray<TSharedRef<ITextDecorator>> Decorators;
    RichTextMarshaller = FRichTextLayoutMarshaller::Create(Decorators, &FAppStyle::Get());
 
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(10)
        [
            SNew(STextBlock).Text(LOCTEXT("Title", "智能自动化导入工具 V5.6")).Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2) [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(LOCTEXT("Src", "源文件夹: ")).MinDesiredWidth(80) ]
                + SHorizontalBox::Slot().FillWidth(1.0f) [ SAssignNew(SourcePathBox, SEditableTextBox).IsReadOnly(true) ]
                + SHorizontalBox::Slot().AutoWidth() [ SNew(SButton).Text(LOCTEXT("B1", "浏览")).OnClicked(this, &SImport_MM::OnBrowseSourceClicked) ]
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2) [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(LOCTEXT("Dst", "目标位置: ")).MinDesiredWidth(80) ]
                + SHorizontalBox::Slot().FillWidth(1.0f) [ SAssignNew(DestPathBox, SEditableTextBox).HintText(LOCTEXT("Hint", "/Game/BatchImport/")) ]
                + SHorizontalBox::Slot().AutoWidth() [ SNew(SButton).Text(LOCTEXT("B2", "项目目录")).OnClicked(this, &SImport_MM::OnBrowseDestClicked) ]
            ]
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(10, 10)
        [
            SNew(SButton).HAlign(HAlign_Center).Text(LOCTEXT("Run", "导入")).OnClicked(this, &SImport_MM::OnStartImportClicked).ContentPadding(FMargin(40, 5))
        ]
        + SVerticalBox::Slot().FillHeight(1.0f).Padding(10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight() [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(1.0f) [ SNew(STextBlock).Text(LOCTEXT("LogLabel", "任务日志:")).ColorAndOpacity(FSlateColor(FLinearColor::Gray)) ]
                + SHorizontalBox::Slot().AutoWidth() [ SNew(SButton).Text(LOCTEXT("Clear", "清空日志")).OnClicked(this, &SImport_MM::OnClearLog) ]
            ]
            + SVerticalBox::Slot().FillHeight(1.0f).Padding(0, 5) [
                SNew(SBorder).BorderImage(FAppStyle::GetBrush("Menu.Background")) [
                    SAssignNew(LogScrollBox, SScrollBox)
                    + SScrollBox::Slot() [
                        SAssignNew(LogBox, SMultiLineEditableText)
                        .Marshaller(RichTextMarshaller) // This enables rich text color parsing
                        .IsReadOnly(true)
                        .AutoWrapText(true)
                    ]
                ]
            ]
        ]
    ];
}
 
void SImport_MM::AddLog(const FString& Message, FLinearColor Color)
{
    FString TimeStr = FDateTime::Now().ToString(TEXT("[%H:%M:%S] "));
    FString ColorHex = Color.ToFColor(true).ToHex();
    
    // 构建富文本颜色标签
    FString FormattedMessage = FString::Printf(TEXT("<RichTextBlock.Color=\"#%s\">%s%s</>\n"), *ColorHex, *TimeStr, *Message);
    
    FText Current = LogBox->GetText();
    FString NewContentStr = Current.ToString() + FormattedMessage;
    
    LogBox->SetText(FText::FromString(NewContentStr));
    LogScrollBox->ScrollToEnd();
}
 
FReply SImport_MM::OnClearLog()
{
    LogBox->SetText(FText::GetEmpty());
    return FReply::Handled();
}
 
FReply SImport_MM::OnBrowseSourceClicked()
{
    IDesktopPlatform* DP = FDesktopPlatformModule::Get();
    if (DP)
    {
        IMainFrameModule& MF = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
        void* Parent = MF.GetParentWindow().IsValid() ? MF.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle() : nullptr;
        FString Out; if (DP->OpenDirectoryDialog(Parent, TEXT("选择源"), TEXT(""), Out)) { SourceFolderPath = Out; SourcePathBox->SetText(FText::FromString(SourceFolderPath)); }
    }
    return FReply::Handled();
}
 
FReply SImport_MM::OnBrowseDestClicked()
{
    IDesktopPlatform* DP = FDesktopPlatformModule::Get();
    if (DP)
    {
        IMainFrameModule& MF = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
        void* Parent = MF.GetParentWindow().IsValid() ? MF.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle() : nullptr;
        FString Out; if (DP->OpenDirectoryDialog(Parent, TEXT("目标"), FPaths::ProjectContentDir(), Out))
        {
            FString Pkg; if (FPackageName::TryConvertFilenameToLongPackageName(Out, Pkg)) { RelativeDestPath = Pkg; DestPathBox->SetText(FText::FromString(RelativeDestPath)); }
        }
    }
    return FReply::Handled();
}
 
FReply SImport_MM::OnStartImportClicked()
{
    if (SourceFolderPath.IsEmpty()) return FReply::Handled();
    FString FinalDest = DestPathBox->GetText().ToString().IsEmpty() ? TEXT("/Game/BatchImport") : DestPathBox->GetText().ToString();
 
    IFileManager& FM = IFileManager::Get();
    auto Scan = [&](const FString& Path, const FString& Name) {
        FImportFolderTask Task; Task.FolderName = Name; TArray<FString> Files; FM.FindFiles(Files, *Path, nullptr);
        for (const FString& F : Files) {
            FString Ext = FPaths::GetExtension(F).ToLower();
            // --- 核心修改：同时支持 fbx 和 obj ---
            if (Ext == TEXT("fbx") || Ext == TEXT("obj")) Task.MeshPath = Path / F;
            else if (Ext == TEXT("png") || Ext == TEXT("tga") || Ext == TEXT("jpg")) Task.TextureMap.Add(F, Path / F);
        }
        if (!Task.MeshPath.IsEmpty()) ExecuteImportTask(Task, FinalDest);
    };
 
    Scan(SourceFolderPath, FPaths::GetBaseFilename(SourceFolderPath));
    TArray<FString> Subs; FM.FindFiles(Subs, *(SourceFolderPath / TEXT("*")), false, true);
    for (const FString& S : Subs) Scan(SourceFolderPath / S, S);
    return FReply::Handled();
}
 
void SImport_MM::ExecuteImportTask(const FImportFolderTask& Task, const FString& BaseDestPath)
{
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    FString FinalPath = BaseDestPath / Task.FolderName;
    FString MeshBaseName = FPaths::GetBaseFilename(Task.MeshPath);
    FString MeshPrefix = MeshBaseName + TEXT("_"); // 剥离前缀
 
    // 1. 模型导入（UFbxFactory 支持 FBX/OBJ 为静态网格）
    UFbxFactory* Fact = NewObject<UFbxFactory>();
    Fact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    Fact->ImportUI->bImportMaterials = Fact->ImportUI->bImportTextures = false;
    Fact->ImportUI->StaticMeshImportData->bTransformVertexToAbsolute = true;
    Fact->ImportUI->StaticMeshImportData->bConvertSceneUnit = true;
    Fact->ImportUI->StaticMeshImportData->ImportUniformScale = 1.0f;
 
    UAssetImportTask* MTask = NewObject<UAssetImportTask>();
    MTask->Filename = Task.MeshPath; MTask->DestinationPath = FinalPath;
    MTask->Factory = Fact; MTask->bAutomated = true; // 消除弹窗
    AT.ImportAssetTasks({ MTask });
 
    // 2. 贴图导入
    TArray<FString> TPaths; Task.TextureMap.GenerateValueArray(TPaths);
    if (TPaths.Num() > 0) AT.ImportAssets(TPaths, FinalPath, nullptr);
    ARM.Get().ScanPathsSynchronous({ FinalPath });
 
    // 3. 获取模型资产并检查自适应缩放
    TArray<FAssetData> MeshDatas; ARM.Get().GetAssetsByPath(FName(*FinalPath), MeshDatas);
    TArray<UStaticMesh*> Meshes;
    for (const FAssetData& Ad : MeshDatas) if (UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset())) Meshes.Add(M);
    if (Meshes.Num() == 0) return;
 
    if (Meshes[0]->GetBounds().GetBox().GetSize().GetMax() < 2.0f) {
        Fact->ImportUI->StaticMeshImportData->ImportUniformScale = 100.0f;
        AT.ImportAssetTasks({ MTask });
        ARM.Get().ScanPathsSynchronous({ FinalPath });
        Meshes.Empty(); ARM.Get().GetAssetsByPath(FName(*FinalPath), MeshDatas);
        for (const FAssetData& Ad : MeshDatas) if (UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset())) Meshes.Add(M);
    }
 
    // 4. 创建材质集
    TMap<FString, UMaterial*> CreatedMaterials;
    UMaterial* SingleFallbackMat = nullptr;
    int32 BaseColorCount = 0;
 
    TArray<FString> BaseColorFileNames;
    for (auto& TP : Task.TextureMap) {
        FString LN = TP.Key.ToLower();
        if (LN.Contains(TEXT("base")) || LN.Contains(TEXT("albedo")) || LN.Contains(TEXT("col"))) {
            BaseColorFileNames.Add(FPaths::GetBaseFilename(TP.Key));
            BaseColorCount++;
        }
    }
 
    for (const FString& BCName : BaseColorFileNames) {
        UMaterialFactoryNew* MF = NewObject<UMaterialFactoryNew>();
        UMaterial* NewMat = Cast<UMaterial>(AT.CreateAsset(TEXT("M_") + BCName, FinalPath, UMaterial::StaticClass(), MF));
        if (!NewMat) continue;
        if (!SingleFallbackMat) SingleFallbackMat = NewMat;
        CreatedMaterials.Add(BCName, NewMat);
 
        int32 YPos = 0;
        TArray<FString> Cats = { TEXT("BC"), TEXT("N"), TEXT("ORM"), TEXT("OP"), TEXT("SP"), TEXT("EM"), TEXT("WPO") };
        TMap<FString, UTexture2D*> LocalMatch;
 
        for (auto& TP : Task.TextureMap) {
            FString TN = FPaths::GetBaseFilename(TP.Key);
            if (BaseColorCount > 1 && !TN.Contains(BCName.Replace(TEXT("_BaseColor"), TEXT(""), ESearchCase::IgnoreCase))) continue;
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *(FinalPath / TN + TEXT(".") + TN));
            if (!Tex) continue;
            FString L = TN.ToLower();
            if (L.Contains(TEXT("base")) || L.Contains(TEXT("albedo")) || L.Contains(TEXT("col"))) LocalMatch.Add(TEXT("BC"), Tex);
            else if (L.Contains(TEXT("normal")) || L.Contains(TEXT("nrm"))) LocalMatch.Add(TEXT("N"), Tex);
            else if (L.Contains(TEXT("rough")) || L.Contains(TEXT("metal")) || L.Contains(TEXT("occ")) || L.Contains(TEXT("ao"))) LocalMatch.Add(TEXT("ORM"), Tex);
            else if (L.Contains(TEXT("opacity")) || L.Contains(TEXT("alpha"))) LocalMatch.Add(TEXT("OP"), Tex);
            else if (L.Contains(TEXT("specular"))) LocalMatch.Add(TEXT("SP"), Tex);
            else if (L.Contains(TEXT("emissive"))) LocalMatch.Add(TEXT("EM"), Tex);
            else if (L.Contains(TEXT("wpo"))) LocalMatch.Add(TEXT("WPO"), Tex);
        }
 
        if (LocalMatch.Contains(TEXT("OP"))) NewMat->BlendMode = BLEND_Translucent;
        for (const FString& K : Cats) {
            if (!LocalMatch.Contains(K)) continue;
            UTexture2D* T = LocalMatch[K];
            bool bN = (K == TEXT("N")), bM = (K == TEXT("ORM") || K == TEXT("OP") || K == TEXT("SP"));
            T->CompressionSettings = bN ? TC_Normalmap : (bM ? TC_Masks : TC_Default);
            T->SRGB = !bN && !bM; T->PostEditChange();
            auto* Node = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
            Node->Texture = T; Node->MaterialExpressionEditorY = YPos; YPos += 300;
            Node->SamplerType = bN ? SAMPLERTYPE_Normal : (bM ? SAMPLERTYPE_Masks : SAMPLERTYPE_Color);
            if (K == TEXT("BC")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_BaseColor);
            else if (K == TEXT("N")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Normal);
            else if (K == TEXT("OP")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Opacity);
            else if (K == TEXT("SP")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Specular);
            else if (K == TEXT("EM")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_EmissiveColor);
            else if (K == TEXT("WPO")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_WorldPositionOffset);
            else if (K == TEXT("ORM")) {
                struct FSort { int32 P; EMaterialProperty Prop; bool operator<(const FSort& O) const { return P < O.P; } };
                TArray<FSort> S; FString LN = T->GetName().ToLower();
                auto AddS = [&](TArray<FString> Ks, EMaterialProperty P){ for(auto& k : Ks){ int32 i = LN.Find(k); if(i != -1){ S.Add({i, P}); break; } } };
                AddS({TEXT("occlusion"), TEXT("ao"), TEXT("occ")}, MP_AmbientOcclusion);
                AddS({TEXT("roughness"), TEXT("rough")}, MP_Roughness);
                AddS({TEXT("metallic"), TEXT("metal")}, MP_Metallic);
                S.Sort(); FString P[] = { TEXT("R"), TEXT("G"), TEXT("B") };
                for (int32 i=0; i<S.Num() && i<3; ++i) UMaterialEditingLibrary::ConnectMaterialProperty(Node, P[i], S[i].Prop);
            }
        }
        UMaterialEditingLibrary::RecompileMaterial(NewMat);
    }
 
    // 5. 分配材质并按总模型记录日志
    bool bHasNamingError = false;
    for (UStaticMesh* SM : Meshes) {
        FString FullName = SM->GetName();
        FString Clean = FullName.StartsWith(MeshPrefix) ? FullName.RightChop(MeshPrefix.Len()) : FullName;
        bool bAssigned = false;
        for (auto& MatPair : CreatedMaterials) {
            if (MatPair.Key.Contains(Clean, ESearchCase::IgnoreCase) || Clean.Contains(MatPair.Key, ESearchCase::IgnoreCase)) {
                for (int32 i=0; i<SM->GetStaticMaterials().Num(); ++i) SM->SetMaterial(i, MatPair.Value);
                SM->PostEditChange(); bAssigned = true; break;
            }
        }
        if (!bAssigned) {
            if (BaseColorCount == 1 && SingleFallbackMat) {
                for (int32 i=0; i<SM->GetStaticMaterials().Num(); ++i) SM->SetMaterial(i, SingleFallbackMat);
                SM->PostEditChange();
                AddLog(FString::Printf(TEXT("模型 [%s] 已检测到拆分出的单独模型中部分或全部模型命名不规范，且现有纹理贴图仅能生成一个材质球。所以将此唯一材质球赋予全部模型。"), *FullName), FLinearColor::Yellow);
            } else {
                bHasNamingError = true;
            }
        }
    }
 
    // 只有当该 FBX/OBJ 整体存在匹配失败时，按原始文件名汇总打印一次红色日志
    if (bHasNamingError) {
        AddLog(FString::Printf(TEXT("错误：模型 [%s] (路径:%s) 无法自动匹配贴图！已生成材质但未赋予。"), *MeshBaseName, *FinalPath), FLinearColor::Red);
    }
}