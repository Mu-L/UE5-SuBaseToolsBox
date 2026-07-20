
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
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionConstant3Vector.h"
#include "Materials/MaterialExpressionStaticSwitchParameter.h"
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
        + SVerticalBox::Slot().AutoHeight().Padding(5)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth()
            [
                SAssignNew(bCreateMICheckbox, SCheckBox)
                // 默认不勾选
            ]
            + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
            [
                SNew(STextBlock).Text(LOCTEXT("CreateMI", "创建实例并应用"))
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
                    // 【关键逻辑】如果勾选了“自动创建实例”，则此项变暗禁用
                    .IsEnabled_Lambda([this](){ return !bCreateMICheckbox->IsChecked(); }) 
                ]
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("UseMI", "使用父材质实例"))
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
    bool bUseMI = bUseParentMICheckbox.IsValid() ? bUseParentMICheckbox->IsChecked() : false;
    UMaterialInstance* ParentMI = Cast<UMaterialInstance>(SelectedParentMIPath.TryLoad());
 
    // 1. 获取 SM6 安全的引擎内置兜底贴图 (保留您原始资源路径)
    UTexture2D* DefNormal = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineMaterials/BaseFlattenNormalMap.BaseFlattenNormalMap"));
    UTexture2D* DefBlackColor = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/Black.Black"));
    UTexture2D* DefBlackLinear = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_LinearColor.UVEditorColorGrid_LinearColor"));
    UTexture2D* DefWhiteLinear = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_Mask.UVEditorColorGrid_Mask"));
 
    // 2. 统计 BaseColor 以确定需要生成的材质数量
    OutBaseColorCount = 0;
    TArray<FString> BaseColorFileNames;
    for (auto& TP : Task.TextureMap) 
    {
        FString Key = TP.Key.ToLower();
        if (Key.Contains(TEXT("base")) || Key.Contains(TEXT("albedo")) || Key.Contains(TEXT("col"))) 
        {
            BaseColorFileNames.Add(FPaths::GetBaseFilename(TP.Key));
            OutBaseColorCount++;
        }
    }
 
    // 3. 为每个检测到的 BaseColor 生成材质
    for (const FString& BCName : BaseColorFileNames) 
    {
        TMap<FString, UTexture2D*> LocalMatch;
        UMaterialInterface* WorkingMat = nullptr;
 
        // --- 匹配当前子材质的贴图文件 ---
        for (auto& TP : Task.TextureMap) 
        {
            FString TN = FPaths::GetBaseFilename(TP.Key);
            
            if (OutBaseColorCount > 1) {
                FString Clean = BCName.Replace(TEXT("_BaseColor"), TEXT(""), ESearchCase::IgnoreCase)
                                      .Replace(TEXT("_Albedo"), TEXT(""), ESearchCase::IgnoreCase)
                                      .Replace(TEXT("_Col"), TEXT(""), ESearchCase::IgnoreCase);
                if (!TN.Contains(Clean)) continue;
            }
 
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *(FinalPath / TN + TEXT(".") + TN));
            if (!Tex) continue;
 
            FString L = TN.ToLower();
            if (L.Contains(TEXT("base")) || L.Contains(TEXT("albedo"))) LocalMatch.Add(TEXT("BC"), Tex);
            else if (L.Contains(TEXT("normal")) || L.Contains(TEXT("_n"))) LocalMatch.Add(TEXT("N"), Tex);
            else if (L.Contains(TEXT("specular"))) LocalMatch.Add(TEXT("SP"), Tex);
            else if (L.Contains(TEXT("aniso"))) LocalMatch.Add(TEXT("AN"), Tex);
            else if (L.Contains(TEXT("opacity")) || L.Contains(TEXT("alpha")) || L.Contains(TEXT("mask"))) LocalMatch.Add(TEXT("OP"), Tex);
            else if (L.Contains(TEXT("emissive"))) LocalMatch.Add(TEXT("EM"), Tex);
            else if (L.Contains(TEXT("orm")) || L.Contains(TEXT("rough")) || L.Contains(TEXT("metal")) || L.Contains(TEXT("ao"))) 
            {
                if (!L.Contains(TEXT("normal"))) LocalMatch.Add(TEXT("ORM"), Tex);
            }
        }
 
        // --- 应用逻辑：材质实例 (MI) 模式 ---
        if (bUseMI && ParentMI)
        {
            UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(AT.DuplicateAsset(TEXT("MI_") + BCName, FinalPath, ParentMI));
            if (!MIC) continue;
            WorkingMat = MIC;
 
            auto ApplyChannel = [&](FString LocalKey, FName SwitchName, FString DefTexName, UTexture2D* Fallback, bool bLinear) {
                bool bFound = LocalMatch.Contains(LocalKey);
                MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(SwitchName), bFound);
                UTexture2D* TargetTex = bFound ? LocalMatch[LocalKey] : Fallback;
                
                if (TargetTex) {
                    if (LocalKey == TEXT("N")) TargetTex->CompressionSettings = TC_Normalmap;
                    else if (LocalKey == TEXT("ORM") || bLinear) TargetTex->CompressionSettings = TC_Masks;
                    
                    TargetTex->SRGB = (LocalKey == TEXT("BC") || LocalKey == TEXT("EM")) ? true : false;
                    TargetTex->PostEditChange();
 
                    FString PName = ParamNameInputs.Contains(LocalKey) ? ParamNameInputs[LocalKey]->GetText().ToString() : DefTexName;
                    MIC->SetTextureParameterValueEditorOnly(FName(*PName), TargetTex);
                }
            };
 
            ApplyChannel(TEXT("BC"), TEXT("Use_BaseColor"), TEXT("BaseColor"), DefBlackColor, false);
            ApplyChannel(TEXT("N"), TEXT("Use_Normal"), TEXT("Normal"), DefNormal, false);
            ApplyChannel(TEXT("EM"), TEXT("Use_Emissive"), TEXT("Emissive"), DefBlackColor, false);
            ApplyChannel(TEXT("SP"), TEXT("Use_Specular"), TEXT("Specular"), DefBlackLinear, true);
            ApplyChannel(TEXT("AN"), TEXT("Use_Anisotropy"), TEXT("Anisotropy"), DefBlackLinear, true);
            ApplyChannel(TEXT("OP"), TEXT("Use_Opacity"), TEXT("Opacity"), DefWhiteLinear, true);
 
            bool bFoundORM = LocalMatch.Contains(TEXT("ORM"));
            bool bAO = false, bRough = false, bMetal = false;
            UTexture2D* ORMTex = bFoundORM ? LocalMatch[TEXT("ORM")] : DefWhiteLinear;
            
            if (bFoundORM) {
                ORMTex->CompressionSettings = TC_Masks; 
                ORMTex->SRGB = false; 
                ORMTex->PostEditChange();
                
                FString N = ORMTex->GetName().ToLower();
                bAO = N.Contains(TEXT("ao")) || N.Contains(TEXT("orm")) || N.Contains(TEXT("occ"));
                bRough = N.Contains(TEXT("rough")) || N.Contains(TEXT("orm"));
                bMetal = N.Contains(TEXT("metal")) || N.Contains(TEXT("orm"));
            }
            
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_AO")), bAO);
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Roughness")), bRough);
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Metallic")), bMetal);
            MIC->SetTextureParameterValueEditorOnly(TEXT("ORM"), ORMTex);
 
            if (LocalMatch.Contains(TEXT("OP"))) {
                MIC->BasePropertyOverrides.bOverride_BlendMode = true;
                MIC->BasePropertyOverrides.BlendMode = BLEND_Masked;
            }
            MIC->PostEditChange();
        }
        // --- 应用逻辑：新建母材质模式 ---
        else
        {
            UMaterial* NewMat = Cast<UMaterial>(AT.CreateAsset(TEXT("M_") + BCName, FinalPath, UMaterial::StaticClass(), NewObject<UMaterialFactoryNew>()));
            if (NewMat) {
                WorkingMat = NewMat;
                if (LocalMatch.Contains(TEXT("OP"))) NewMat->BlendMode = BLEND_Masked;
 
                int32 YPos = 0;
                for (auto& Pair : LocalMatch) {
                    UTexture2D* T = Pair.Value;
                    auto* Node = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                    Node->Texture = T; 
                    Node->MaterialExpressionEditorY = YPos; 
                    YPos += 350;
                    
                    FString K = Pair.Key;
                    
                    // --- 关键修正：修改贴图资产属性以匹配采样器，消除 SM6 报错 ---
                    if (K == TEXT("N")) 
                    {
                        T->SRGB = false;
                        T->CompressionSettings = TC_Normalmap;
                        Node->SamplerType = SAMPLERTYPE_Normal;
                    }
                    else if (K == TEXT("BC") || K == TEXT("EM")) 
                    {
                        T->SRGB = true;
                        T->CompressionSettings = TC_Default;
                        Node->SamplerType = SAMPLERTYPE_Color;
                    }
                    else if (K == TEXT("ORM"))
                    {
                        T->SRGB = false;
                        T->CompressionSettings = TC_Masks;
                        Node->SamplerType = SAMPLERTYPE_Masks;
                    }
                    else // SP, AN, OP
                    {
                        T->SRGB = false;
                        T->CompressionSettings = TC_Masks;
                        Node->SamplerType = SAMPLERTYPE_Masks;
                    }
                    
                    // 应用资产修改
                    T->PostEditChange();
 
                    // 建立连线
                    if (K == TEXT("BC")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_BaseColor);
                    else if (K == TEXT("N")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Normal);
                    else if (K == TEXT("EM")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_EmissiveColor);
                    else if (K == TEXT("OP")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_OpacityMask);
                    else if (K == TEXT("ORM")) {
                        UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT("R"), MP_AmbientOcclusion);
                        UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT("G"), MP_Roughness);
                        UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT("B"), MP_Metallic);
                    }
                }
                UMaterialEditingLibrary::RecompileMaterial(NewMat);
                WorkingMat = NewMat; // 默认工作材质是母材质
 
                // 【新增逻辑】判断是否开启了自动实例化
                if (bCreateMICheckbox.IsValid() && bCreateMICheckbox->IsChecked())
                {
                    FString MIName = TEXT("MI_") + BCName;
                    UMaterialInstanceConstantFactoryNew* MIFact = NewObject<UMaterialInstanceConstantFactoryNew>();
            
                    // 在同一路径下创建材质实例
                    UMaterialInstanceConstant* NewMIC = Cast<UMaterialInstanceConstant>(
                        AT.CreateAsset(MIName, FinalPath, UMaterialInstanceConstant::StaticClass(), MIFact)
                    );
 
                    if (NewMIC)
                    {
                        NewMIC->SetParentEditorOnly(NewMat); // 设置刚刚生成的母材质为父级
                        NewMIC->PostEditChange();
                
                        // 【关键】将 WorkingMat 指向实例，这样 OutCreatedMaterials 存入的就是实例
                        WorkingMat = NewMIC; 
                
                        AddLog(FString::Printf(TEXT("已自动生成并应用材质实例: %s"), *MIName), FLinearColor::Green);
                    }
                }
            }
        }
 
        if (!OutSingleFallbackMat) OutSingleFallbackMat = WorkingMat;
        OutCreatedMaterials.Add(BCName, WorkingMat);
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
 
    // 预加载引擎资源（保留您原始定义的路径）
    UTexture2D* DefNormal = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineMaterials/BaseFlattenNormalMap.BaseFlattenNormalMap"));
    UTexture2D* DefBlackColor = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/Black.Black"));
    UTexture2D* DefBlackLinear = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_LinearColor.UVEditorColorGrid_LinearColor"));
    UTexture2D* DefWhiteLinear = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_Mask.UVEditorColorGrid_Mask"));
 
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
    UMaterial* MasterMat = Cast<UMaterial>(AT.CreateAsset(TEXT("M_AdvancedBatchMaster_Final"), TargetPath, UMaterial::StaticClass(), MatFact));
    if (!MasterMat) return FReply::Handled();
 
    UMaterialEditorOnlyData* MatEditorData = MasterMat->GetEditorOnlyData();
 
    // 创建常量节点
    auto* ConstZero = NewObject<UMaterialExpressionConstant>(MasterMat); 
    MasterMat->GetExpressionCollection().AddExpression(ConstZero);
    auto* ConstOne = NewObject<UMaterialExpressionConstant>(MasterMat); 
    ConstOne->R = 1.0f; 
    MasterMat->GetExpressionCollection().AddExpression(ConstOne);
    auto* ConstHalf = NewObject<UMaterialExpressionConstant>(MasterMat); 
    ConstHalf->R = 0.5f; 
    MasterMat->GetExpressionCollection().AddExpression(ConstHalf);
    auto* ConstNormal = NewObject<UMaterialExpressionConstant3Vector>(MasterMat); 
    ConstNormal->Constant = FLinearColor(0, 0, 1); 
    MasterMat->GetExpressionCollection().AddExpression(ConstNormal);
 
    auto CreateChannel = [&](FName TexName, FName SwName, int32 Y, EMaterialSamplerType Sampler, UTexture2D* DefTex, UMaterialExpression* Fallback, FExpressionInput& TargetPin)
    {
        auto* TexNode = NewObject<UMaterialExpressionTextureSampleParameter2D>(MasterMat);
        TexNode->ParameterName = TexName; 
        TexNode->SamplerType = Sampler; 
        TexNode->Texture = DefTex; // 必须设置一个非空默认值
        TexNode->MaterialExpressionEditorX = -1000; 
        TexNode->MaterialExpressionEditorY = Y;
        MasterMat->GetExpressionCollection().AddExpression(TexNode);
 
        auto* SwNode = NewObject<UMaterialExpressionStaticSwitchParameter>(MasterMat);
        SwNode->ParameterName = SwName; 
        SwNode->MaterialExpressionEditorX = -500; 
        SwNode->MaterialExpressionEditorY = Y;
        MasterMat->GetExpressionCollection().AddExpression(SwNode);
 
        SwNode->A.Expression = TexNode;
        SwNode->B.Expression = Fallback;
        TargetPin.Expression = SwNode;
    };
 
    // 构建图表
    CreateChannel(TEXT("BaseColor"), TEXT("Use_BaseColor"), 0, SAMPLERTYPE_Color, DefBlackColor, ConstZero, MatEditorData->BaseColor);
    CreateChannel(TEXT("Normal"), TEXT("Use_Normal"), 350, SAMPLERTYPE_Normal, DefNormal, ConstNormal, MatEditorData->Normal);
    CreateChannel(TEXT("Emissive"), TEXT("Use_Emissive"), 700, SAMPLERTYPE_Color, DefBlackColor, ConstZero, MatEditorData->EmissiveColor);
    CreateChannel(TEXT("Specular"), TEXT("Use_Specular"), 1050, SAMPLERTYPE_LinearColor, DefBlackLinear, ConstHalf, MatEditorData->Specular);
    CreateChannel(TEXT("Anisotropy"), TEXT("Use_Anisotropy"), 1400, SAMPLERTYPE_LinearColor, DefBlackLinear, ConstZero, MatEditorData->Anisotropy);
    
    // 透明度通道连接
    CreateChannel(TEXT("Opacity"), TEXT("Use_Opacity"), 1750, SAMPLERTYPE_Masks, DefWhiteLinear, ConstOne, MatEditorData->OpacityMask);
 
    // ORM
    auto* ORMTex = NewObject<UMaterialExpressionTextureSampleParameter2D>(MasterMat);
    ORMTex->ParameterName = TEXT("ORM"); ORMTex->SamplerType = SAMPLERTYPE_Masks; ORMTex->Texture = DefWhiteLinear;
    ORMTex->MaterialExpressionEditorX = -1200; ORMTex->MaterialExpressionEditorY = 2100;
    MasterMat->GetExpressionCollection().AddExpression(ORMTex);
 
    auto LinkORM = [&](FName SwName, int32 OutIdx, UMaterialExpression* Fall, FExpressionInput& Pin, int32 Y) {
        auto* Sw = NewObject<UMaterialExpressionStaticSwitchParameter>(MasterMat);
        Sw->ParameterName = SwName; Sw->MaterialExpressionEditorX = -500; Sw->MaterialExpressionEditorY = Y;
        MasterMat->GetExpressionCollection().AddExpression(Sw);
        Sw->A.Expression = ORMTex; Sw->A.OutputIndex = OutIdx;
        Sw->B.Expression = Fall; Pin.Expression = Sw;
    };
    LinkORM(TEXT("Use_AO"), 1, ConstOne, MatEditorData->AmbientOcclusion, 2100);
    LinkORM(TEXT("Use_Roughness"), 2, ConstOne, MatEditorData->Roughness, 2400);
    LinkORM(TEXT("Use_Metallic"), 3, ConstZero, MatEditorData->Metallic, 2700);
 
    MasterMat->PostEditChange();
    UMaterialEditingLibrary::RecompileMaterial(MasterMat);
 
    FString InstanceName = TEXT("MI_AdvancedBatchMaster_Final");
    UMaterialInstanceConstantFactoryNew* MIFact = NewObject<UMaterialInstanceConstantFactoryNew>();
    UMaterialInstanceConstant* NewMI = Cast<UMaterialInstanceConstant>(AT.CreateAsset(InstanceName, TargetPath, UMaterialInstanceConstant::StaticClass(), MIFact));
    if (NewMI) {
        NewMI->SetParentEditorOnly(MasterMat);
        NewMI->PostEditChange();
        SelectedParentMIPath = FSoftObjectPath(NewMI);
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