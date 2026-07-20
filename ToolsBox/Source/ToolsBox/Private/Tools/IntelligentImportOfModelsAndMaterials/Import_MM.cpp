
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
#include "Slate_Assist/FIconStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SExpandableArea.h"
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
                SNew(SScrollBox)
               + SScrollBox::Slot().Padding(10)
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
                    + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
                    [
                        SNew(SExpandableArea)
                       .AreaTitle(LOCTEXT("NamingRules", "资产命名详细规则"))
                       .InitiallyCollapsed(true)
                       .BodyContent()
                       [
            
                           SNew(SBorder)
                           .Padding(FMargin(10, 5))
                           .BorderImage(FIconStyle::Get_Images().GetBrush("ToolsBox.Image_Anon_1K")) 
                           [
                               SNew(SVerticalBox)
                               + SVerticalBox::Slot().AutoHeight() [ CreateNamingRow(EImportAssetType::Mesh, TEXT("[ 模型 ]"), TEXT("SM_")) ]
                               + SVerticalBox::Slot().AutoHeight() [ CreateNamingRow(EImportAssetType::Texture, TEXT("[ 贴图 ]"), TEXT("T_")) ]
                               + SVerticalBox::Slot().AutoHeight() [ CreateNamingRow(EImportAssetType::Material, TEXT("[ 母材质 ]"), TEXT("M_")) ]
                               + SVerticalBox::Slot().AutoHeight() [ CreateNamingRow(EImportAssetType::Instance, TEXT("[ 材质实例 ]"), TEXT("MI_")) ]
                           ]
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
                            SNew(SBorder)
                            [
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
                       
                   ]
                
            ];
            
    
    
}

TSharedRef<SWidget> SImport_MM::CreateNamingRow(EImportAssetType Type, const FString& Label, const FString& DefaultPrefix)
{
    FNamingWidgets Widgets;
 
    // 创建该资产类型的垂直布局组
    TSharedRef<SVerticalBox> ContentBox = SNew(SVerticalBox);
 
    // 第一行：资产类别标题（如：模型、贴图）
    ContentBox->AddSlot().AutoHeight().Padding(0, 5, 0, 2)
    [
        SNew(STextBlock)
        .Text(FText::FromString(Label))
        .Font(FAppStyle::GetFontStyle("DetailsView.CategoryFontStyle"))
        .ColorAndOpacity(FLinearColor(0.4f, 0.8f, 1.0f)) // 淡蓝色标识类别
    ];
 
    // 第二行：前缀行
    ContentBox->AddSlot().AutoHeight().Padding(15, 2) // 向右缩进
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
        [
            SAssignNew(Widgets.bUsePrefix, SCheckBox).IsChecked(ECheckBoxState::Checked)
        ]
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(5, 0)
        [
            SNew(STextBlock).Text(LOCTEXT("PrefixLabel", "前缀:")).MinDesiredWidth(40)
        ]
        + SHorizontalBox::Slot().FillWidth(1.0f)
        [
            SAssignNew(Widgets.PrefixEntry, SEditableTextBox)
            .Text(FText::FromString(DefaultPrefix))
            .Visibility_Lambda([this, Type]() { 
                return NamingControlMap.Contains(Type) && NamingControlMap[Type].bUsePrefix->IsChecked() ? EVisibility::Visible : EVisibility::Hidden; 
            })
        ]
    ];
 
    // 第三行：后缀行
    ContentBox->AddSlot().AutoHeight().Padding(15, 2, 0, 8)
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
        [
            SAssignNew(Widgets.bUseSuffix, SCheckBox).IsChecked(ECheckBoxState::Unchecked)
        ]
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(5, 0)
        [
            SNew(STextBlock).Text(LOCTEXT("SuffixLabel", "后缀:")).MinDesiredWidth(40)
        ]
        + SHorizontalBox::Slot().FillWidth(1.0f)
        [
            SAssignNew(Widgets.SuffixEntry, SEditableTextBox)
            .HintText(LOCTEXT("SuffixHint", "输入后缀内容..."))
            .Visibility_Lambda([this, Type]() { 
                return NamingControlMap.Contains(Type) && NamingControlMap[Type].bUseSuffix->IsChecked() ? EVisibility::Visible : EVisibility::Hidden; 
            })
        ]
    ];
 
    NamingControlMap.Add(Type, Widgets);
    return ContentBox;
}
 
FString SImport_MM::GetAppliedName(const FString& RawName, EImportAssetType Type)
{
    if (!NamingControlMap.Contains(Type)) return RawName;
    
    const FNamingWidgets& Widgets = NamingControlMap[Type];
    FString FinalName = RawName;
 
    if (Widgets.bUsePrefix->IsChecked()) FinalName = Widgets.PrefixEntry->GetText().ToString() + FinalName;
    if (Widgets.bUseSuffix->IsChecked()) FinalName = FinalName + Widgets.SuffixEntry->GetText().ToString();
 
    return FinalName;
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
    
    // 获取原始文件名并应用命名规则
    FString RawMeshName = FPaths::GetBaseFilename(Task.MeshPath);
    FString AppliedMeshName = GetAppliedName(RawMeshName, EImportAssetType::Mesh);
 
    UFbxFactory* Fact = NewObject<UFbxFactory>();
    Fact->ImportUI->MeshTypeToImport = FBXIT_StaticMesh;
    Fact->ImportUI->bImportMaterials = Fact->ImportUI->bImportTextures = false;
    Fact->ImportUI->StaticMeshImportData->bTransformVertexToAbsolute = true;
    Fact->ImportUI->StaticMeshImportData->bConvertSceneUnit = true;
    Fact->ImportUI->StaticMeshImportData->ImportUniformScale = 1.0f;
 
    UAssetImportTask* MTask = NewObject<UAssetImportTask>();
    MTask->Filename = Task.MeshPath; 
    MTask->DestinationPath = FinalPath;
    // 【关键修改】：明确指定导入后的资产名称，应用前缀和后缀
    MTask->DestinationName = AppliedMeshName; 
    MTask->Factory = Fact; 
    MTask->bAutomated = true;
    
    AT.ImportAssetTasks({ MTask });
 
    AddLog(FString::Printf(TEXT("模型导入任务已提交: %s"), *AppliedMeshName), FLinearColor::White);
}
 
 
void SImport_MM::PerformTextureImport(const FImportFolderTask& Task, const FString& FinalPath)
{
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
 
    TArray<UAssetImportTask*> TextureTasks;
 
    // 遍历任务中的每一张贴图
    for (auto& TP : Task.TextureMap)
    {
        FString RawTexName = FPaths::GetBaseFilename(TP.Key);
        // 【关键修改】：对贴图应用命名规则
        FString AppliedTexName = GetAppliedName(RawTexName, EImportAssetType::Texture);
 
        UAssetImportTask* TTask = NewObject<UAssetImportTask>();
        TTask->Filename = TP.Value; // 原始文件全路径
        TTask->DestinationPath = FinalPath;
        TTask->DestinationName = AppliedTexName; // 应用命名规则后的名称
        TTask->bAutomated = true;
        TTask->bSave = true;
 
        TextureTasks.Add(TTask);
    }
    
    if (TextureTasks.Num() > 0)
    {
        AT.ImportAssetTasks(TextureTasks);
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
    
    bool bUseExistingParent = bUseParentMICheckbox.IsValid() ? bUseParentMICheckbox->IsChecked() : false;
    bool bAutoCreateInstance = bCreateMICheckbox.IsValid() ? bCreateMICheckbox->IsChecked() : false;
    UMaterialInstance* ParentMI = Cast<UMaterialInstance>(SelectedParentMIPath.TryLoad());
 
    UTexture2D* DefNormal = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineMaterials/BaseFlattenNormalMap.BaseFlattenNormalMap"));
    UTexture2D* DefBlackColor = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/Black.Black"));
    UTexture2D* DefWhiteLinear = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_Mask.UVEditorColorGrid_Mask"));
 
    OutBaseColorCount = 0;
    TArray<FString> BaseColorFileNames;
    for (auto& TP : Task.TextureMap) 
    {
        FString Key = FPaths::GetBaseFilename(TP.Key).ToLower();
        if (Key.Contains(TEXT("base")) || Key.Contains(TEXT("albedo")) || Key.Contains(TEXT("col"))) 
        {
            BaseColorFileNames.Add(FPaths::GetBaseFilename(TP.Key));
            OutBaseColorCount++;
        }
    }
 
    for (const FString& BCName : BaseColorFileNames) 
    {
        TMap<FString, UTexture2D*> LocalMatch;
        UMaterialInterface* WorkingMat = nullptr;
        bool bNeedTransparency = false; // 是否需要开启透明模式
 
        for (auto& TP : Task.TextureMap) 
        {
            FString RawTexFileName = FPaths::GetBaseFilename(TP.Key);
            if (OutBaseColorCount > 1) {
                FString Clean = BCName.Replace(TEXT("_BaseColor"), TEXT(""), ESearchCase::IgnoreCase)
                                      .Replace(TEXT("_Albedo"), TEXT(""), ESearchCase::IgnoreCase)
                                      .Replace(TEXT("_Col"), TEXT(""), ESearchCase::IgnoreCase);
                if (!RawTexFileName.Contains(Clean)) continue;
            }
 
            FString AppliedTexAssetName = GetAppliedName(RawTexFileName, EImportAssetType::Texture);
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *(FinalPath / AppliedTexAssetName + TEXT(".") + AppliedTexAssetName));
            if (!Tex) continue;
 
            FString L = RawTexFileName.ToLower();
            if (L.Contains(TEXT("base")) || L.Contains(TEXT("albedo"))) LocalMatch.Add(TEXT("BC"), Tex);
            else if (L.Contains(TEXT("normal")) || L.Contains(TEXT("_n"))) LocalMatch.Add(TEXT("N"), Tex);
            else if (L.Contains(TEXT("emissive"))) LocalMatch.Add(TEXT("EM"), Tex);
            else if (L.Contains(TEXT("orm")) || L.Contains(TEXT("rough")) || L.Contains(TEXT("metal")) || L.Contains(TEXT("ao"))) 
            {
                if (!L.Contains(TEXT("normal"))) LocalMatch.Add(TEXT("ORM"), Tex);
            }
            else if (L.Contains(TEXT("opacity")) || L.Contains(TEXT("alpha")) || L.Contains(TEXT("mask")) || L.Contains(TEXT("_op"))) 
            {
                LocalMatch.Add(TEXT("OP"), Tex);
                bNeedTransparency = true; // 识别到透明贴图，标记需要透明模式
            }
        }
 
        if (bUseExistingParent && ParentMI)
        {
            FString MIName = GetAppliedName(BCName, EImportAssetType::Instance);
            UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(AT.DuplicateAsset(MIName, FinalPath, ParentMI));
            
            if (MIC)
            {
                WorkingMat = MIC;
 
                // 通道参数设置 Lambda
                auto SetParam = [&](FString LocalKey, FName SwitchName, FString DefaultParamName) {
                    bool bExists = LocalMatch.Contains(LocalKey);
                    MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(SwitchName), bExists);
                    
                    if (bExists)
                    {
                        UTexture2D* Tex = LocalMatch[LocalKey];
                        if (LocalKey == TEXT("N")) { Tex->SRGB = false; Tex->CompressionSettings = TC_Normalmap; }
                        else if (LocalKey == TEXT("BC") || LocalKey == TEXT("EM")) { Tex->SRGB = true; Tex->CompressionSettings = TC_Default; }
                        else { Tex->SRGB = false; Tex->CompressionSettings = TC_Masks; }
                        Tex->PostEditChange();
 
                        FString FinalParamName = ParamNameInputs.Contains(LocalKey) ? ParamNameInputs[LocalKey]->GetText().ToString() : DefaultParamName;
                        MIC->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(FName(*FinalParamName)), Tex);
                    }
                };
 
                SetParam(TEXT("BC"), TEXT("Use_BaseColor"), TEXT("BaseColor"));
                SetParam(TEXT("N"), TEXT("Use_Normal"), TEXT("Normal"));
                SetParam(TEXT("EM"), TEXT("Use_Emissive"), TEXT("Emissive"));
                SetParam(TEXT("OP"), TEXT("Use_Opacity"), TEXT("Opacity"));
 
                // ORM 处理
                bool bFoundORM = LocalMatch.Contains(TEXT("ORM"));
                MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_ORM")), bFoundORM);
                MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_AO")), bFoundORM);
                MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Roughness")), bFoundORM);
                MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Metallic")), bFoundORM);
                
                if (bFoundORM) {
                    UTexture2D* ORMTex = LocalMatch[TEXT("ORM")];
                    ORMTex->SRGB = false;
                    ORMTex->CompressionSettings = TC_Masks;
                    ORMTex->PostEditChange();
                    FString ORMPName = ParamNameInputs.Contains(TEXT("ORM")) ? ParamNameInputs[TEXT("ORM")]->GetText().ToString() : TEXT("ORM");
                    MIC->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(FName(*ORMPName)), ORMTex);
                }
 
                // 【核心修正】：自动切换材质实例的 Blend Mode
                if (bNeedTransparency)
                {
                    // 开启混合模式覆盖
                    MIC->BasePropertyOverrides.bOverride_BlendMode = true;
                    // 设置为 Masked 模式（性能最好且支持镂空透明）
                    MIC->BasePropertyOverrides.BlendMode = BLEND_Masked;
                    AddLog(FString::Printf(TEXT("材质实例 [%s] 检测到透明贴图，已自动开启 Masked 混合模式。"), *MIName), FLinearColor::Green);
                }
 
                MIC->UpdateStaticPermutation();
                MIC->PostEditChange();
            }
        }
        else
        {
            // 生成新材质逻辑
            FString FinalMatName = GetAppliedName(BCName, EImportAssetType::Material);
            UMaterial* NewMat = Cast<UMaterial>(AT.CreateAsset(FinalMatName, FinalPath, UMaterial::StaticClass(), NewObject<UMaterialFactoryNew>()));
            if (NewMat) 
            {
                WorkingMat = NewMat;
                
                // 【核心修正】：新生成的母材质自动设置 BlendMode
                if (bNeedTransparency) 
                {
                    NewMat->BlendMode = BLEND_Masked;
                }
 
                int32 YPos = 0;
                for (auto& Pair : LocalMatch) 
                {
                    UTexture2D* T = Pair.Value;
                    auto* Node = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                    Node->Texture = T; 
                    Node->MaterialExpressionEditorY = YPos; 
                    YPos += 350;
                    FString K = Pair.Key;
                    if (K == TEXT("N")) { T->SRGB = false; T->CompressionSettings = TC_Normalmap; Node->SamplerType = SAMPLERTYPE_Normal; }
                    else if (K == TEXT("BC") || K == TEXT("EM")) { T->SRGB = true; T->CompressionSettings = TC_Default; Node->SamplerType = SAMPLERTYPE_Color; }
                    else { T->SRGB = false; T->CompressionSettings = TC_Masks; Node->SamplerType = SAMPLERTYPE_Masks; }
                    T->PostEditChange();
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
                if (bAutoCreateInstance) {
                    FString FinalInstName = GetAppliedName(BCName, EImportAssetType::Instance);
                    UMaterialInstanceConstant* NewMIC = Cast<UMaterialInstanceConstant>(AT.CreateAsset(FinalInstName, FinalPath, UMaterialInstanceConstant::StaticClass(), NewObject<UMaterialInstanceConstantFactoryNew>()));
                    if (NewMIC) { NewMIC->SetParentEditorOnly(NewMat); NewMIC->PostEditChange(); WorkingMat = NewMIC; }
                }
            }
        }
        if (!OutSingleFallbackMat) OutSingleFallbackMat = WorkingMat;
        OutCreatedMaterials.Add(BCName, WorkingMat);
    }
}
 
void SImport_MM::ApplyMaterialsToMeshes(const TArray<UStaticMesh*>& Meshes, const TMap<FString, UMaterialInterface*>& CreatedMaterials, int32 BaseColorCount, UMaterialInterface* SingleFallbackMat, const FString& MeshBaseName, const FString& FinalPath)
{
    // 关键修正：获取模型在应用命名规则后的预期名称前缀
    const FString AppliedMeshBaseName = GetAppliedName(MeshBaseName, EImportAssetType::Mesh);
    const FString MeshSearchPrefix = AppliedMeshBaseName + TEXT("_");
    
    bool bHasNamingError = false;
 
    for (UStaticMesh* SM : Meshes) 
    {
        // 1. 获取模型当前在引擎中的真实名称
        FString CurrentSMName = SM->GetName();
        
        // 2. 清理名称：去掉前缀部分，还原出原始逻辑名（用于和 CreatedMaterials 的 Key 匹配）
        // 例如：从 "SM_Chair_Seat" 还原出 "Seat"
        FString CleanLogicName = CurrentSMName.StartsWith(MeshSearchPrefix) ? CurrentSMName.RightChop(MeshSearchPrefix.Len()) : CurrentSMName;
        
        // 如果连 AppliedMeshBaseName 都包含了，也清理掉
        if (CleanLogicName.StartsWith(AppliedMeshBaseName))
        {
             CleanLogicName = CleanLogicName.RightChop(AppliedMeshBaseName.Len()).TrimStartAndEnd().Replace(TEXT("_"), TEXT(""), ESearchCase::IgnoreCase);
        }
 
        bool bAssigned = false;
 
        // 3. 遍历已创建的材质映射表（注意：CreatedMaterials 的 Key 通常是原始贴图名，如 "Chair_BC"）
        for (auto& MatPair : CreatedMaterials) 
        {
            FString MatKey = MatPair.Key.ToLower();
            FString TargetMatchName = CleanLogicName.ToLower();
 
            // 模糊匹配逻辑：如果模型名包含材质名，或材质名包含模型名
            if (MatKey.Contains(TargetMatchName) || TargetMatchName.Contains(MatKey) || TargetMatchName.IsEmpty()) 
            {
                for (int32 i = 0; i < SM->GetStaticMaterials().Num(); ++i)
                {
                    SM->SetMaterial(i, MatPair.Value);
                }
                SM->PostEditChange(); 
                bAssigned = true; 
                break;
            }
        }
 
        // 4. 兜底逻辑：如果没匹配上但只有一个材质，直接赋予
        if (!bAssigned) 
        {
            if (BaseColorCount == 1 && SingleFallbackMat) 
            {
                for (int32 i = 0; i < SM->GetStaticMaterials().Num(); ++i)
                {
                    SM->SetMaterial(i, SingleFallbackMat);
                }
                SM->PostEditChange();
                bAssigned = true;
                AddLog(FString::Printf(TEXT("模型 [%s] 自动适配唯一材质。"), *CurrentSMName), FLinearColor::Yellow);
            } 
            else 
            {
                bHasNamingError = true;
            }
        }
    }
 
    if (bHasNamingError) 
    {
        AddLog(FString::Printf(TEXT("提醒：模型 [%s] 部分组件未能自动匹配到对应材质，请手动检查。"), *AppliedMeshBaseName), FLinearColor::Red);
    }
    else
    {
        AddLog(FString::Printf(TEXT("成功：已为 [%s] 及其组件完成材质分配。"), *AppliedMeshBaseName), FLinearColor::Green);
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