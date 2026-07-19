
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
    TArray<TSharedRef<ITextDecorator>> Decorators;

    TSharedPtr<FRichTextLayoutMarshaller> LocalMarshaller = FRichTextLayoutMarshaller::Create(Decorators, &FAppStyle::Get());
 
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(10)
        [
            SNew(STextBlock).Text(LOCTEXT("Title", "批量导入工具 V1")).Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2) [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(LOCTEXT("Src", "源文件夹: ")).MinDesiredWidth(100) ]
                + SHorizontalBox::Slot().FillWidth(1.0f) [ SAssignNew(SourcePathBox, SEditableTextBox).IsReadOnly(true) ]
                + SHorizontalBox::Slot().AutoWidth() [ SNew(SButton).Text(LOCTEXT("B1", "浏览")).OnClicked(this, &SImport_MM::OnBrowseSourceClicked) ]
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2) [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(LOCTEXT("Dst", "保存位置: ")).MinDesiredWidth(100) ]
                + SHorizontalBox::Slot().FillWidth(1.0f) [ SAssignNew(DestPathBox, SEditableTextBox).HintText(LOCTEXT("Hint", "/Game/BatchImport/")) ]
                + SHorizontalBox::Slot().AutoWidth() [ SNew(SButton).Text(LOCTEXT("B2", "选择路径")).OnClicked(this, &SImport_MM::OnBrowseDestClicked) ]
            ]
            +SVerticalBox::Slot()
            .Padding(1,20,1,1)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("Tip", "如果纹理贴图不与模型同一层级下，而是处于同级的文件夹内，请输入统一的此文件夹名称。留空则默认贴图与模型同级。 "))
                .AutoWrapText(true)
               
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2) [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                [
                    
                    SNew(STextBlock)
                    .Text(LOCTEXT("TexFolder", "贴图子文件夹名: ")).MinDesiredWidth(100)
                    
                ]
                + SHorizontalBox::Slot().FillWidth(1.0f) [ SAssignNew(TexSubFolderNameBox, SEditableTextBox).HintText(LOCTEXT("TexHint", "留空则在模型同级目录找贴图")) ]
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
                        .Marshaller(LocalMarshaller)
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
    FString FormattedMessage = FString::Printf(TEXT("<RichTextBlock.Color=\"#%s\">%s%s</>\n"), *ColorHex, *TimeStr, *Message);
    FText Current = LogBox->GetText();
    LogBox->SetText(FText::FromString(Current.ToString() + FormattedMessage));
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
    FString TexSubFolderName = TexSubFolderNameBox->GetText().ToString();
 
    AddLog(TEXT("--- 开始执行层级递归导入 ---"), FLinearColor::White);
    IFileManager& FM = IFileManager::Get();
    
    TFunction<void(const FString&)> RecursiveScan;
    RecursiveScan = [&](const FString& CurrentDirPath) {
        TArray<FString> LocalFiles;
        FM.FindFiles(LocalFiles, *(CurrentDirPath / TEXT("*")), true, false);
 
        FString RelativePath = CurrentDirPath == SourceFolderPath ? TEXT("") : CurrentDirPath.RightChop(SourceFolderPath.Len() + 1);
 
        for (const FString& FileName : LocalFiles) {
            FString Ext = FPaths::GetExtension(FileName).ToLower();
            if (Ext == TEXT("fbx") || Ext == TEXT("obj")) {
                FImportFolderTask Task;
                Task.MeshPath = CurrentDirPath / FileName;
                Task.FolderName = RelativePath.IsEmpty() ? FPaths::GetBaseFilename(SourceFolderPath) : RelativePath;
                
                FString TexSearchPath = TexSubFolderName.IsEmpty() ? CurrentDirPath : (CurrentDirPath / TexSubFolderName);
 
                TArray<FString> TexFiles;
                FM.FindFiles(TexFiles, *(TexSearchPath / TEXT("*")), true, false);
                for (const FString& TF : TexFiles) {
                    FString TExt = FPaths::GetExtension(TF).ToLower();
                    if (TExt == TEXT("png") || TExt == TEXT("tga") || TExt == TEXT("jpg")) {
                        Task.TextureMap.Add(TF, TexSearchPath / TF);
                    }
                }
                ExecuteImportTask(Task, FinalDest);
            }
        }
 
        TArray<FString> SubDirs;
        FM.FindFiles(SubDirs, *(CurrentDirPath / TEXT("*")), false, true);
        for (const FString& SubDirName : SubDirs) {
            // 排除指定的贴图子目录，避免扫描其内部的模型
            if (!TexSubFolderName.IsEmpty() && SubDirName.Equals(TexSubFolderName, ESearchCase::IgnoreCase)) continue;
            RecursiveScan(CurrentDirPath / SubDirName);
        }
    };
 
    RecursiveScan(SourceFolderPath);
    AddLog(TEXT("--- 所有导入任务已结束 ---"), FLinearColor::White);
    return FReply::Handled();
}
 
void SImport_MM::ExecuteImportTask(const FImportFolderTask& Task, const FString& BaseDestPath)
{
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    
    FString FinalPath = BaseDestPath / Task.FolderName;
    FString MeshBaseName = FPaths::GetBaseFilename(Task.MeshPath);
    FString MeshPrefix = MeshBaseName + TEXT("_");
 
    // 1. 模型导入配置
    UFbxFactory* Fact = NewObject<UFbxFactory>();
    Fact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    Fact->ImportUI->bImportMaterials = Fact->ImportUI->bImportTextures = false;
    Fact->ImportUI->StaticMeshImportData->bTransformVertexToAbsolute = true;
    Fact->ImportUI->StaticMeshImportData->bConvertSceneUnit = true;
    Fact->ImportUI->StaticMeshImportData->ImportUniformScale = 1.0f;
 
    UAssetImportTask* MTask = NewObject<UAssetImportTask>();
    MTask->Filename = Task.MeshPath; 
    MTask->DestinationPath = FinalPath;
    MTask->Factory = Fact; 
    MTask->bAutomated = true;
    AT.ImportAssetTasks({ MTask });
 
    // 2. 贴图导入
    TArray<FString> TPaths; Task.TextureMap.GenerateValueArray(TPaths);
    if (TPaths.Num() > 0) AT.ImportAssets(TPaths, FinalPath, nullptr);
    ARM.Get().ScanPathsSynchronous({ FinalPath });
 
    // 3. 获取并过滤当前任务生成的 Mesh
    TArray<FAssetData> MeshDatas; ARM.Get().GetAssetsByPath(FName(*FinalPath), MeshDatas);
    TArray<UStaticMesh*> Meshes;
    for (const FAssetData& Ad : MeshDatas) {
        UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset());
        // 确保是该文件生成的 Mesh，防止被 FinalPath 下的其他模型污染
        if (M && M->GetName().Contains(MeshBaseName)) Meshes.Add(M);
    }
    if (Meshes.Num() == 0) return;
 
    // 4. 分析贴图并创建材质
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
        TMap<FString, UTexture2D*> LocalMatch;
        for (auto& TP : Task.TextureMap) {
            FString TN = FPaths::GetBaseFilename(TP.Key);
            // 多贴图集时执行前缀过滤
            if (BaseColorCount > 1 && !TN.Contains(BCName.Replace(TEXT("_BaseColor"), TEXT(""), ESearchCase::IgnoreCase))) continue;
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *(FinalPath / TN + TEXT(".") + TN));
            if (!Tex) continue;
 
            FString L = TN.ToLower();
            if (L.Contains(TEXT("base")) || L.Contains(TEXT("albedo"))) LocalMatch.Add(TEXT("BC"), Tex);
            else if (L.Contains(TEXT("normal"))) LocalMatch.Add(TEXT("N"), Tex);
            else if (L.Contains(TEXT("rough")) || L.Contains(TEXT("metal")) || L.Contains(TEXT("occ")) || L.Contains(TEXT("ao"))) LocalMatch.Add(TEXT("ORM"), Tex);
            else if (L.Contains(TEXT("opacity"))) LocalMatch.Add(TEXT("OP"), Tex);
            else if (L.Contains(TEXT("specular"))) LocalMatch.Add(TEXT("SP"), Tex);
            else if (L.Contains(TEXT("emissive"))) LocalMatch.Add(TEXT("EM"), Tex);
            else if (L.Contains(TEXT("wpo"))) LocalMatch.Add(TEXT("WPO"), Tex);
        }
 
        if (LocalMatch.Contains(TEXT("OP"))) NewMat->BlendMode = BLEND_Translucent;
        for (auto& Pair : LocalMatch) {
            UTexture2D* T = Pair.Value; FString K = Pair.Key;
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
 
    // 5. 分配材质与汇总日志
    bool bHasNamingError = false;
    for (UStaticMesh* SM : Meshes) {
        FString Clean = SM->GetName().StartsWith(MeshPrefix) ? SM->GetName().RightChop(MeshPrefix.Len()) : SM->GetName();
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
                AddLog(FString::Printf(TEXT("模型 [%s] 已检测到拆分出的单独模型中部分或全部模型命名不规范，且现有纹理贴图仅能生成一个材质球。所以将此唯一材质球赋予全部模型。"), *SM->GetName()), FLinearColor::Yellow);
            } else bHasNamingError = true;
        }
    }
 
    if (bHasNamingError) {
        AddLog(FString::Printf(TEXT("错误：模型 [%s] (路径:%s) 无法自动匹配贴图！已生成材质但未赋予。"), *MeshBaseName, *FinalPath), FLinearColor::Red);
    }
}