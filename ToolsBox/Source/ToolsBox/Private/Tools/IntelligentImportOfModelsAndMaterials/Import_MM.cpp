
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
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Framework/Text/ITextDecorator.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Interfaces/IMainFrameModule.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"

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
        + SVerticalBox::Slot().AutoHeight().Padding(0, 5)
        [
        SNew(SBorder).BorderImage(FAppStyle::GetBrush("DetailsView.CategoryTop"))
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SAssignNew(bUseParentMICheckbox, SCheckBox)
                    .OnCheckStateChanged(this, &SImport_MM::OnUseParentMIToggled)
                ]
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("UseMI", "基于父材质实例创建 (勾选则不生成新材质)"))
                ]
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SAssignNew(ParentMISelector, SObjectPropertyEntryBox)
                .AllowedClass(UMaterialInstance::StaticClass())
                .ObjectPath_Lambda([this](){ return SelectedParentMIPath.ToString(); })
                .OnObjectChanged_Lambda([this](const FAssetData& AssetData){ 
                    SelectedParentMIPath = AssetData.GetSoftObjectPath(); 
                })
                .IsEnabled_Lambda([this](){ return bUseParentMICheckbox->IsChecked(); })
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
            [
                SNew(SButton)
                .HAlign(HAlign_Center)
                .Text(LOCTEXT("CreateGenMat", "生成通用父材质及实例"))
                .ToolTipText(LOCTEXT("CreateGenMatTip", "自动创建一个带有标准参数(BaseColor, Normal, ORM)的父材质及其对应的实例"))
                .OnClicked(this, &SImport_MM::OnCreateGenericMaterialClicked)
                .ContentPadding(FMargin(10, 2))
            ]
            // 贴图参数名配置区
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SGridPanel)
                .FillColumn(1, 1.0f)
                + SGridPanel::Slot(0, 0).Padding(2) [ CreateParamInputRow(TEXT("基础颜色:"), TEXT("BaseColor"), TEXT("BC")) ]
                + SGridPanel::Slot(1, 0).Padding(2) [ CreateParamInputRow(TEXT("法线:"), TEXT("Normal"), TEXT("N")) ]
                + SGridPanel::Slot(0, 1).Padding(2) [ CreateParamInputRow(TEXT("ORM/遮罩:"), TEXT("ORM"), TEXT("ORM")) ]
                + SGridPanel::Slot(1, 1).Padding(2) [ CreateParamInputRow(TEXT("自发光:"), TEXT("Emissive"), TEXT("EM")) ]
            ]
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
    const FString FinalPath = BaseDestPath / Task.FolderName;
    const FString MeshBaseName = FPaths::GetBaseFilename(Task.MeshPath);
 
    // 阶段 1: 导入模型
    PerformMeshImport(Task, FinalPath);
 
    // 阶段 2: 导入贴图
    PerformTextureImport(Task, FinalPath);
 
    // 阶段 3: 获取导入的模型对象
    TArray<UStaticMesh*> Meshes = CollectImportedMeshes(FinalPath, MeshBaseName);
    if (Meshes.Num() == 0) return;
 
    // --- 关键修正点：将局部变量类型改为 UMaterialInterface* ---
    TMap<FString, UMaterialInterface*> CreatedMaterials; 
    UMaterialInterface* SingleFallbackMat = nullptr;
    int32 BaseColorCount = 0;
 
    // 阶段 4: 分析贴图并创建材质 (现在类型匹配了，引用绑定成功)
    GenerateMaterials(Task, FinalPath, CreatedMaterials, SingleFallbackMat, BaseColorCount);
 
    // 阶段 5: 分配材质 (同时更新此函数内部处理逻辑以支持 Interface)
    ApplyMaterialsToMeshes(Meshes, CreatedMaterials, BaseColorCount, SingleFallbackMat, MeshBaseName, FinalPath);
}


void SImport_MM::PerformMeshImport(const FImportFolderTask& Task, const FString& FinalPath)
{
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    
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
}
 
void SImport_MM::PerformTextureImport(const FImportFolderTask& Task, const FString& FinalPath)
{
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
 
    TArray<FString> TPaths; 
    Task.TextureMap.GenerateValueArray(TPaths);
    
    if (TPaths.Num() > 0)
    {
        AT.ImportAssets(TPaths, FinalPath, nullptr);
    }
    ARM.Get().ScanPathsSynchronous({ FinalPath });
}
 
TArray<UStaticMesh*> SImport_MM::CollectImportedMeshes(const FString& FinalPath, const FString& MeshBaseName)
{
    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> MeshDatas; 
    ARM.Get().GetAssetsByPath(FName(*FinalPath), MeshDatas);
    
    TArray<UStaticMesh*> Meshes;
    for (const FAssetData& Ad : MeshDatas) 
    {
        UStaticMesh* M = Cast<UStaticMesh>(Ad.GetAsset());
        if (M && M->GetName().Contains(MeshBaseName)) 
        {
            Meshes.Add(M);
        }
    }
    return Meshes;
}
 
void SImport_MM::GenerateMaterials(const FImportFolderTask& Task, const FString& FinalPath, 
    TMap<FString, UMaterialInterface*>& OutCreatedMaterials, UMaterialInterface*& OutSingleFallbackMat, int32& OutBaseColorCount)
{
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    
    // 检查模式：是新建材质还是实例化父材质
    bool bUseMI = bUseParentMICheckbox.IsValid() ? bUseParentMICheckbox->IsChecked() : false;
    UMaterialInstance* ParentMI = Cast<UMaterialInstance>(SelectedParentMIPath.TryLoad());

    if (bUseMI && !ParentMI)
    {
        AddLog(TEXT("错误：勾选了使用父材质实例但未选择有效的资产！将尝试回退至默认生成模式。"), FLinearColor::Red);
        bUseMI = false;
    }

    OutBaseColorCount = 0;
    TArray<FString> BaseColorFileNames;

    // 1. 提取 BaseColor 作为材质生成的基准
    for (auto& TP : Task.TextureMap) 
    {
        FString LN = TP.Key.ToLower();
        if (LN.Contains(TEXT("base")) || LN.Contains(TEXT("albedo")) || LN.Contains(TEXT("col"))) 
        {
            BaseColorFileNames.Add(FPaths::GetBaseFilename(TP.Key));
            OutBaseColorCount++;
        }
    }
 
    // 2. 遍历并创建材质资源
    for (const FString& BCName : BaseColorFileNames) 
    {
        UMaterialInterface* WorkingMat = nullptr;

        if (bUseMI && ParentMI)
        {
            // 模式 A: 复制并创建材质实例 (MIC)
            FString NewMIName = TEXT("MI_") + BCName;
            // 使用 DuplicateAsset 基于父类创建一个新的实例资产
            WorkingMat = Cast<UMaterialInstanceConstant>(AT.DuplicateAsset(NewMIName, FinalPath, ParentMI));
        }
        else
        {
            // 模式 B: 创建全新的材质资源
            UMaterialFactoryNew* MF = NewObject<UMaterialFactoryNew>();
            WorkingMat = Cast<UMaterial>(AT.CreateAsset(TEXT("M_") + BCName, FinalPath, UMaterial::StaticClass(), MF));
        }

        if (!WorkingMat) continue;
        if (!OutSingleFallbackMat) OutSingleFallbackMat = WorkingMat;
        OutCreatedMaterials.Add(BCName, WorkingMat);
 
        // 3. 匹配当前材质对应的贴图文件
        int32 YPos = 0;
        TMap<FString, UTexture2D*> LocalMatch;
        for (auto& TP : Task.TextureMap) 
        {
            FString TN = FPaths::GetBaseFilename(TP.Key);
            // 如果存在多套贴图，根据命名进行过滤匹配
            if (OutBaseColorCount > 1 && !TN.Contains(BCName.Replace(TEXT("_BaseColor"), TEXT(""), ESearchCase::IgnoreCase))) continue;
            
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
 
        // 4. 应用贴图数据
        if (bUseMI)
        {
            // 子模式 A: 填充材质实例的参数
            UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(WorkingMat);
            for (auto& Pair : LocalMatch)
            {
                // 从 UI 输入框获取用户定义的变量名，若无则使用 Key 作为缺省
                FString ParamName = ParamNameInputs.Contains(Pair.Key) ? ParamNameInputs[Pair.Key]->GetText().ToString() : Pair.Key;
                if (!ParamName.IsEmpty())
                {
                    MIC->SetTextureParameterValueEditorOnly(FName(*ParamName), Pair.Value);
                }
            }
            MIC->PostEditChange();
        }
        else
        {
            // 子模式 B: 传统的节点连线逻辑 (原有逻辑)
            UMaterial* NewMat = Cast<UMaterial>(WorkingMat);
            if (LocalMatch.Contains(TEXT("OP"))) NewMat->BlendMode = BLEND_Translucent;

            for (auto& Pair : LocalMatch) 
            {
                UTexture2D* T = Pair.Value; 
                FString K = Pair.Key;
                bool bN = (K == TEXT("N")), bM = (K == TEXT("ORM") || K == TEXT("OP") || K == TEXT("SP"));
                
                // 自动调整贴图压缩设置，以消除 (Eliminate) 渲染错误
                T->CompressionSettings = bN ? TC_Normalmap : (bM ? TC_Masks : TC_Default);
                T->SRGB = !bN && !bM; 
                T->PostEditChange();

                auto* Node = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                Node->Texture = T; 
                Node->MaterialExpressionEditorY = YPos; 
                YPos += 300;
                Node->SamplerType = bN ? SAMPLERTYPE_Normal : (bM ? SAMPLERTYPE_Masks : SAMPLERTYPE_Color);

                if (K == TEXT("BC")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_BaseColor);
                else if (K == TEXT("N")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Normal);
                else if (K == TEXT("OP")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Opacity);
                else if (K == TEXT("SP")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Specular);
                else if (K == TEXT("EM")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_EmissiveColor);
                else if (K == TEXT("WPO")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_WorldPositionOffset);
                else if (K == TEXT("ORM")) 
                {
                    struct FSort { int32 P; EMaterialProperty Prop; bool operator<(const FSort& O) const { return P < O.P; } };
                    TArray<FSort> S; FString LN = T->GetName().ToLower();
                    auto AddS = [&](TArray<FString> Ks, EMaterialProperty P){ for(auto& k : Ks){ int32 i = LN.Find(k); if(i != -1){ S.Add({i, P}); break; } } };
                    AddS({TEXT("occlusion"), TEXT("ao"), TEXT("occ")}, MP_AmbientOcclusion);
                    AddS({TEXT("roughness"), TEXT("rough")}, MP_Roughness);
                    AddS({TEXT("metallic"), TEXT("metal")}, MP_Metallic);
                    S.Sort(); 
                    FString P[] = { TEXT("R"), TEXT("G"), TEXT("B") };
                    for (int32 i=0; i<S.Num() && i<3; ++i) UMaterialEditingLibrary::ConnectMaterialProperty(Node, P[i], S[i].Prop);
                }
            }
            UMaterialEditingLibrary::RecompileMaterial(NewMat);
        }
    }
}
 
void SImport_MM::ApplyMaterialsToMeshes(const TArray<UStaticMesh*>& Meshes, const TMap<FString, UMaterialInterface*>& CreatedMaterials, int32 BaseColorCount, UMaterialInterface* SingleFallbackMat,const FString& MeshBaseName, const FString& FinalPath)
{
    const FString MeshPrefix = MeshBaseName + TEXT("_");
    bool bHasNamingError = false;
 
    for (UStaticMesh* SM : Meshes) 
    {
        FString Clean = SM->GetName().StartsWith(MeshPrefix) ? SM->GetName().RightChop(MeshPrefix.Len()) : SM->GetName();
        bool bAssigned = false;
 
        for (auto& MatPair : CreatedMaterials) 
        {
            if (MatPair.Key.Contains(Clean, ESearchCase::IgnoreCase) || Clean.Contains(MatPair.Key, ESearchCase::IgnoreCase)) 
            {
                for (int32 i=0; i<SM->GetStaticMaterials().Num(); ++i) SM->SetMaterial(i, MatPair.Value);
                SM->PostEditChange(); 
                bAssigned = true; 
                break;
            }
        }
 
        if (!bAssigned) 
        {
            if (BaseColorCount == 1 && SingleFallbackMat) 
            {
                for (int32 i=0; i<SM->GetStaticMaterials().Num(); ++i) SM->SetMaterial(i, SingleFallbackMat);
                SM->PostEditChange();
                AddLog(FString::Printf(TEXT("模型 [%s] 已检测到拆分出的单独模型中部分或全部模型命名不规范，且现有纹理贴图仅能生成一个材质球。已赋予唯一材质。"), *SM->GetName()), FLinearColor::Yellow);
            } 
            else 
            {
                bHasNamingError = true;
            }
        }
    }
 
    if (bHasNamingError) 
    {
        AddLog(FString::Printf(TEXT("错误：模型 [%s] (路径:%s) 无法自动匹配贴图！已生成材质但未赋予。"), *MeshBaseName, *FinalPath), FLinearColor::Red);
    }
}


TSharedRef<SWidget> SImport_MM::CreateParamInputRow(const FString& ChannelLabel, const FString& DefaultParamName, const FString& Key)
{
    TSharedPtr<SEditableTextBox> InputBox;
    TSharedRef<SWidget> Widget = SNew(SHorizontalBox)
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center) [ SNew(STextBlock).Text(FText::FromString(ChannelLabel)).MinDesiredWidth(60) ]
        + SHorizontalBox::Slot().FillWidth(1.0f) [ SAssignNew(InputBox, SEditableTextBox).Text(FText::FromString(DefaultParamName)) ];
    
    ParamNameInputs.Add(Key, InputBox);
    return Widget;
}

void SImport_MM::OnUseParentMIToggled(ECheckBoxState NewState)
{
    bool bUseParent = (NewState == ECheckBoxState::Checked);
    
    if (bUseParent)
    {
        AddLog(TEXT("已切换至 [材质实例实例化] 模式。请确保下方选择了有效的父材质实例，并填写了正确的参数名称。"), FLinearColor::Green);
    }
    else
    {
        AddLog(TEXT("已切换至 [新材质生成] 模式。程序将自动创建材质节点并连接。"), FLinearColor::Green);
    }
}


FReply SImport_MM::OnCreateGenericMaterialClicked()
 {
    FString TargetPath = DestPathBox->GetText().ToString();
    if (TargetPath.IsEmpty()) TargetPath = TEXT("/Game/BatchImport");

    // 提前加载引擎内置的默认贴图，防止采样类型不匹配报错
    UTexture2D* DefaultNormal = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineMaterials/BaseFlattenNormalMap.BaseFlattenNormalMap"));
    UTexture2D* DefaultMasks = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_Mask.UVEditorColorGrid_Mask")); 
    
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    
    // 1. 创建 Master Material
    FString MasterName = TEXT("M_UniversalBatchMaster");
    UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
    UMaterial* MasterMat = Cast<UMaterial>(AT.CreateAsset(MasterName, TargetPath, UMaterial::StaticClass(), MatFact));
 
    if (!MasterMat) return FReply::Handled();
 
    // 2. 添加参数节点并连接
    // 基础颜色
    auto* BCNode = AddTextureParameter(MasterMat, TEXT("BaseColor"), 0, SAMPLERTYPE_Color);
    UMaterialEditingLibrary::ConnectMaterialProperty(BCNode, TEXT(""), MP_BaseColor);
 
    // 法线
    auto* NNode = AddTextureParameter(MasterMat, TEXT("Normal"), 300, SAMPLERTYPE_Normal);
    UMaterialEditingLibrary::ConnectMaterialProperty(NNode, TEXT(""), MP_Normal);
    NNode->Texture=DefaultNormal;

    
    // ORM (R:AO, G:Roughness, B:Metallic)
    auto* ORMNode = AddTextureParameter(MasterMat, TEXT("ORM"), 600, SAMPLERTYPE_Masks);
    UMaterialEditingLibrary::ConnectMaterialProperty(ORMNode, TEXT("R"), MP_AmbientOcclusion);
    UMaterialEditingLibrary::ConnectMaterialProperty(ORMNode, TEXT("G"), MP_Roughness);
    UMaterialEditingLibrary::ConnectMaterialProperty(ORMNode, TEXT("B"), MP_Metallic);
    ORMNode->Texture=DefaultMasks;
    
    // 自发光
    auto* EMNode = AddTextureParameter(MasterMat, TEXT("Emissive"), 900, SAMPLERTYPE_Color);
    UMaterialEditingLibrary::ConnectMaterialProperty(EMNode, TEXT(""), MP_EmissiveColor);
 
    // 编译材质
    UMaterialEditingLibrary::RecompileMaterial(MasterMat);
    MasterMat->PostEditChange();
 
    // 3. 创建 Material Instance
    FString InstanceName = TEXT("MI_UniversalBatchMaster");
    UMaterialInstanceConstantFactoryNew* MIFact = NewObject<UMaterialInstanceConstantFactoryNew>();
    UMaterialInstanceConstant* NewMI = Cast<UMaterialInstanceConstant>(AT.CreateAsset(InstanceName, TargetPath, UMaterialInstanceConstant::StaticClass(), MIFact));
 
    if (NewMI)
    {
        NewMI->SetParentEditorOnly(MasterMat);
        NewMI->PostEditChange();
 
        // 4. 自动填充 UI 状态
        SelectedParentMIPath = FSoftObjectPath(NewMI);
        if (bUseParentMICheckbox.IsValid())
        {
            bUseParentMICheckbox->SetIsChecked(ECheckBoxState::Checked);
        }
 
        AddLog(FString::Printf(TEXT("成功创建父材质 [%s] 和实例 [%s] 并已自动选中。"), *MasterName, *InstanceName), FLinearColor::Green);
    }
 
    return FReply::Handled();
}

UMaterialExpressionTextureSampleParameter2D* SImport_MM::AddTextureParameter(UMaterial* InMaterial, FName InParamName, int32 InYPos, EMaterialSamplerType InSamplerType)
{
    auto* Node = Cast<UMaterialExpressionTextureSampleParameter2D>(
        UMaterialEditingLibrary::CreateMaterialExpression(InMaterial, UMaterialExpressionTextureSampleParameter2D::StaticClass())
    );
    Node->ParameterName = InParamName;
    Node->SamplerType = InSamplerType;
    Node->MaterialExpressionEditorX = -400;
    Node->MaterialExpressionEditorY = InYPos;
    return Node;
}