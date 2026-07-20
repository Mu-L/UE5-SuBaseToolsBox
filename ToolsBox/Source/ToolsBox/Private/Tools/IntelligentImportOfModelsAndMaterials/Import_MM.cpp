
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
    
    // 1. 获取模式设置
    bool bUseMI = bUseParentMICheckbox.IsValid() ? bUseParentMICheckbox->IsChecked() : false;
    UMaterialInstance* ParentMI = Cast<UMaterialInstance>(SelectedParentMIPath.TryLoad());
 
    if (bUseMI && !ParentMI)
    {
        AddLog(TEXT("警告：勾选了实例化但未选中有效父材质。已回退至新建材质模式。"), FLinearColor::Yellow);
        bUseMI = false;
    }
 
    // 2. 统计 BaseColor 作为材质生成的“锚点”
    OutBaseColorCount = 0;
    TArray<FString> BaseColorFileNames;
    for (auto& TP : Task.TextureMap) 
    {
        FString FileName = TP.Key.ToLower();
        if (FileName.Contains(TEXT("base")) || FileName.Contains(TEXT("albedo")) || FileName.Contains(TEXT("col"))) 
        {
            BaseColorFileNames.Add(FPaths::GetBaseFilename(TP.Key));
            OutBaseColorCount++;
        }
    }
 
    // 3. 核心遍历：为每一个检测到的 BaseColor 创建材质
    for (const FString& BCName : BaseColorFileNames) 
    {
        UMaterialInterface* WorkingMat = nullptr;
 
        // --- A. 创建资源资产 ---
        if (bUseMI && ParentMI)
        {
            // 模式：材质实例
            FString NewMIName = TEXT("MI_") + BCName;
            // 通过复制 ParentMI 创建新实例，确保层级结构正确
            WorkingMat = Cast<UMaterialInstanceConstant>(AT.DuplicateAsset(NewMIName, FinalPath, ParentMI));
        }
        else
        {
            // 模式：新建普通材质
            FString NewMatName = TEXT("M_") + BCName;
            UMaterialFactoryNew* MF = NewObject<UMaterialFactoryNew>();
            WorkingMat = Cast<UMaterial>(AT.CreateAsset(NewMatName, FinalPath, UMaterial::StaticClass(), MF));
        }
 
        if (!WorkingMat) continue;
        
        // 记录输出供关联使用
        if (!OutSingleFallbackMat) OutSingleFallbackMat = WorkingMat;
        OutCreatedMaterials.Add(BCName, WorkingMat);
 
        // --- B. 局部贴图搜索与匹配 ---
        TMap<FString, UTexture2D*> LocalMatch;
        for (auto& TP : Task.TextureMap) 
        {
            FString TN = FPaths::GetBaseFilename(TP.Key);
            
            // 如果存在多套贴图，过滤出属于当前 BaseColor 对应的贴图 (基于命名包含关系)
            if (OutBaseColorCount > 1)
            {
                // 去除 BaseColor 常用后缀，提取纯资产名
                FString CleanBCName = BCName.Replace(TEXT("_BaseColor"), TEXT(""), ESearchCase::IgnoreCase)
                                          .Replace(TEXT("_Albedo"), TEXT(""), ESearchCase::IgnoreCase);
                if (!TN.Contains(CleanBCName)) continue;
            }
            
            // 加载导入后的贴图对象
            UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *(FinalPath / TN + TEXT(".") + TN));
            if (!Tex) continue;
 
            FString L = TN.ToLower();
            if (L.Contains(TEXT("base")) || L.Contains(TEXT("albedo"))) LocalMatch.Add(TEXT("BC"), Tex);
            else if (L.Contains(TEXT("normal"))) LocalMatch.Add(TEXT("N"), Tex);
            else if (L.Contains(TEXT("rough")) || L.Contains(TEXT("metal")) || L.Contains(TEXT("occ")) || L.Contains(TEXT("ao"))) LocalMatch.Add(TEXT("ORM"), Tex);
            else if (L.Contains(TEXT("emissive"))) LocalMatch.Add(TEXT("EM"), Tex);
            
            // 特殊关键字：orm 复合贴图
            if (L.Contains(TEXT("orm"))) LocalMatch.Add(TEXT("ORM_TEX"), Tex);
        }
 
        // --- C. 应用参数逻辑 ---
        if (bUseMI)
        {
            // 子模式：填充 Material Instance Constant (MIC)
            UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(WorkingMat);
            
            // 1. 设置 BaseColor 通道
            bool bFoundBC = LocalMatch.Contains(TEXT("BC"));
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_BaseColor")), bFoundBC);
            if (bFoundBC)
            {
                FString PName = ParamNameInputs.Contains(TEXT("BC")) ? ParamNameInputs[TEXT("BC")]->GetText().ToString() : TEXT("BaseColor");
                MIC->SetTextureParameterValueEditorOnly(FName(*PName), LocalMatch[TEXT("BC")]);
            }
 
            // 2. 设置 Normal 通道
            bool bFoundN = LocalMatch.Contains(TEXT("N"));
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Normal")), bFoundN);
            if (bFoundN)
            {
                FString PName = ParamNameInputs.Contains(TEXT("N")) ? ParamNameInputs[TEXT("N")]->GetText().ToString() : TEXT("Normal");
                MIC->SetTextureParameterValueEditorOnly(FName(*PName), LocalMatch[TEXT("N")]);
            }
 
            // 3. 设置 ORM 通道 (AO/Roughness/Metallic)
            bool bAnyORM = LocalMatch.Contains(TEXT("ORM")) || LocalMatch.Contains(TEXT("ORM_TEX"));
            UTexture2D* ORMTex = LocalMatch.Contains(TEXT("ORM_TEX")) ? LocalMatch[TEXT("ORM_TEX")] : (LocalMatch.Contains(TEXT("ORM")) ? LocalMatch[TEXT("ORM")] : nullptr);
            
            // 细分 ORM 每个通道的开关
            bool bAO = false, bRough = false, bMetal = false;
            if (ORMTex)
            {
                FString TexName = ORMTex->GetName().ToLower();
                // 只要有 ORM 贴图，或者文件名包含特定关键字，就开启对应细分开关
                bAO = TexName.Contains(TEXT("ao")) || TexName.Contains(TEXT("orm")) || TexName.Contains(TEXT("occ"));
                bRough = TexName.Contains(TEXT("rough")) || TexName.Contains(TEXT("orm"));
                bMetal = TexName.Contains(TEXT("metal")) || TexName.Contains(TEXT("orm"));
            }
 
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_AO")), bAO);
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Roughness")), bRough);
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Metallic")), bMetal);
 
            if (bAnyORM)
            {
                FString PName = ParamNameInputs.Contains(TEXT("ORM")) ? ParamNameInputs[TEXT("ORM")]->GetText().ToString() : TEXT("ORM");
                MIC->SetTextureParameterValueEditorOnly(FName(*PName), ORMTex);
            }
 
            // 4. 设置 Emissive 通道
            bool bFoundEM = LocalMatch.Contains(TEXT("EM"));
            MIC->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(TEXT("Use_Emissive")), bFoundEM);
            if (bFoundEM)
            {
                FString PName = ParamNameInputs.Contains(TEXT("EM")) ? ParamNameInputs[TEXT("EM")]->GetText().ToString() : TEXT("Emissive");
                MIC->SetTextureParameterValueEditorOnly(FName(*PName), LocalMatch[TEXT("EM")]);
            }
 
            // 关键：强制刷新以确保所有静态开关编译生效
            MIC->PostEditChange();
        }
        else
        {
            // 子模式：新建材质时的传统连线逻辑 (原有模式 B)
            UMaterial* NewMat = Cast<UMaterial>(WorkingMat);
            int32 YPos = 0;
 
            for (auto& Pair : LocalMatch) 
            {
                UTexture2D* T = Pair.Value; 
                FString K = Pair.Key;
                
                // 跳过辅助 Key (ORM_TEX) 避免重复创建节点
                if (K == TEXT("ORM_TEX")) continue;
 
                // 更新贴图属性
                bool bN = (K == TEXT("N")), bM = (K == TEXT("ORM"));
                T->CompressionSettings = bN ? TC_Normalmap : (bM ? TC_Masks : TC_Default);
                T->SRGB = !bN && !bM; 
                T->PostEditChange();
 
                // 创建采样节点
                auto* Node = Cast<UMaterialExpressionTextureSample>(UMaterialEditingLibrary::CreateMaterialExpression(NewMat, UMaterialExpressionTextureSample::StaticClass()));
                Node->Texture = T; 
                Node->MaterialExpressionEditorY = YPos; 
                YPos += 300;
                Node->SamplerType = bN ? SAMPLERTYPE_Normal : (bM ? SAMPLERTYPE_Masks : SAMPLERTYPE_Color);
 
                // 连线
                if (K == TEXT("BC")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_BaseColor);
                else if (K == TEXT("N")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_Normal);
                else if (K == TEXT("EM")) UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT(""), MP_EmissiveColor);
                else if (K == TEXT("ORM")) 
                {
                    // 智能 ORM 分配
                    UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT("R"), MP_AmbientOcclusion);
                    UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT("G"), MP_Roughness);
                    UMaterialEditingLibrary::ConnectMaterialProperty(Node, TEXT("B"), MP_Metallic);
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
 
    UTexture2D* DefNormal = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineMaterials/BaseFlattenNormalMap.BaseFlattenNormalMap"));
    UTexture2D* DefBlack = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/Black.Black"));
    UTexture2D* DefWhite = LoadObject<UTexture2D>(nullptr, TEXT("/UVEditor/Textures/UVEditorColorGrid_Mask.UVEditorColorGrid_Mask"));
 
    IAssetTools& AT = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    UMaterialFactoryNew* MatFact = NewObject<UMaterialFactoryNew>();
    UMaterial* MasterMat = Cast<UMaterial>(AT.CreateAsset(TEXT("M_UniversalBatchMaster_Final"), TargetPath, UMaterial::StaticClass(), MatFact));
    if (!MasterMat) return FReply::Handled();
 
    // 获取编辑器专用数据对象（用于连接根节点引脚）
    UMaterialEditorOnlyData* MatEditorData = MasterMat->GetEditorOnlyData();
 
    // 1. 创建全局公共默认值节点
    auto* ConstZero = NewObject<UMaterialExpressionConstant>(MasterMat);
    MasterMat->GetExpressionCollection().AddExpression(ConstZero);
    
    auto* ConstOne = NewObject<UMaterialExpressionConstant>(MasterMat);
    ConstOne->R = 1.0f;
    MasterMat->GetExpressionCollection().AddExpression(ConstOne);
 
    auto* ConstNormal = NewObject<UMaterialExpressionConstant3Vector>(MasterMat);
    ConstNormal->Constant = FLinearColor(0, 0, 1);
    MasterMat->GetExpressionCollection().AddExpression(ConstNormal);
 
    // --- 内部 Lambda：核心连接逻辑 ---
    auto CreateChannel = [&](FName TexName, FName SwitchName, int32 Y, EMaterialSamplerType Sampler, UTexture2D* DefTex, UMaterialExpression* Fallback, FExpressionInput& TargetRootPin)
    {
        auto* TexNode = NewObject<UMaterialExpressionTextureSampleParameter2D>(MasterMat);
        TexNode->ParameterName = TexName;
        TexNode->SamplerType = Sampler;
        TexNode->Texture = DefTex;
        TexNode->MaterialExpressionEditorX = -1000;
        TexNode->MaterialExpressionEditorY = Y;
        MasterMat->GetExpressionCollection().AddExpression(TexNode);
 
        auto* SwNode = NewObject<UMaterialExpressionStaticSwitchParameter>(MasterMat);
        SwNode->ParameterName = SwitchName;
        SwNode->MaterialExpressionEditorX = -500;
        SwNode->MaterialExpressionEditorY = Y;
        MasterMat->GetExpressionCollection().AddExpression(SwNode);
 
        // 【核心修正】：直接通过 C++ 成员变量连接，不使用字符串
        SwNode->A.Expression = TexNode;     // A 引脚 = True
        SwNode->B.Expression = Fallback;    // B 引脚 = False/Default
        
        TargetRootPin.Expression = SwNode; // 连接到材质根节点
    };
 
    // 2. 连接基础通道
    CreateChannel(TEXT("BaseColor"), TEXT("Use_BaseColor"), 0, SAMPLERTYPE_Color, DefBlack, ConstZero, MatEditorData->BaseColor);
    CreateChannel(TEXT("Normal"), TEXT("Use_Normal"), 350, SAMPLERTYPE_Normal, DefNormal, ConstNormal, MatEditorData->Normal);
    CreateChannel(TEXT("Emissive"), TEXT("Use_Emissive"), 1500, SAMPLERTYPE_Color, DefBlack, ConstZero, MatEditorData->EmissiveColor);
 
    // 3. 特殊处理 ORM 通道 (共享贴图，多个开关)
    auto* ORMTex = NewObject<UMaterialExpressionTextureSampleParameter2D>(MasterMat);
    ORMTex->ParameterName = TEXT("ORM");
    ORMTex->SamplerType = SAMPLERTYPE_Masks;
    ORMTex->Texture = DefWhite;
    ORMTex->MaterialExpressionEditorX = -1200;
    ORMTex->MaterialExpressionEditorY = 800;
    MasterMat->GetExpressionCollection().AddExpression(ORMTex);
 
    auto LinkORM = [&](FName SwName, int32 OutputIndex, UMaterialExpression* Fallback, FExpressionInput& TargetRootPin, int32 Y) {
        auto* Sw = NewObject<UMaterialExpressionStaticSwitchParameter>(MasterMat);
        Sw->ParameterName = SwName;
        Sw->MaterialExpressionEditorX = -500;
        Sw->MaterialExpressionEditorY = Y;
        MasterMat->GetExpressionCollection().AddExpression(Sw);
 
        // A 连接 ORM 贴图的特定通道
        Sw->A.Expression = ORMTex;
        Sw->A.OutputIndex = OutputIndex; // 0=RGB, 1=R, 2=G, 3=B
        
        // B 连接默认值
        Sw->B.Expression = Fallback;
        
        TargetRootPin.Expression = Sw;
    };
 
    LinkORM(TEXT("Use_AO"), 1, ConstOne, MatEditorData->AmbientOcclusion, 750); // R -> AO
    LinkORM(TEXT("Use_Roughness"), 2, ConstOne, MatEditorData->Roughness, 1000); // G -> Roughness
    LinkORM(TEXT("Use_Metallic"), 3, ConstZero, MatEditorData->Metallic, 1250); // B -> Metallic
 
    // 4. 更新与编译
    MasterMat->PreEditChange(nullptr);
    MasterMat->PostEditChange();
    UMaterialEditingLibrary::RecompileMaterial(MasterMat);
 
    // 5. 创建 MI
    FString InstanceName = TEXT("MI_UniversalBatchMaster_Final");
    UMaterialInstanceConstantFactoryNew* MIFact = NewObject<UMaterialInstanceConstantFactoryNew>();
    UMaterialInstanceConstant* NewMI = Cast<UMaterialInstanceConstant>(AT.CreateAsset(InstanceName, TargetPath, UMaterialInstanceConstant::StaticClass(), MIFact));
 
    if (NewMI)
    {
        NewMI->SetParentEditorOnly(MasterMat);
        NewMI->PostEditChange();
        SelectedParentMIPath = FSoftObjectPath(NewMI);
        if (bUseParentMICheckbox.IsValid()) bUseParentMICheckbox->SetIsChecked(ECheckBoxState::Checked);
        AddLog(TEXT("已生成父材质。连接已通过内存指针直接建立，确保视觉图表正确连接。"), FLinearColor::Green);
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