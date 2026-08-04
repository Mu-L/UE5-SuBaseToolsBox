// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "IAssetTools.h"
#include "IContentBrowserSingleton.h"
#include "Selection.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/Material.h"
#include "UObject/Package.h"
#include "Tools/MaterialTttributeTransfer/MaterialTttributeTransfer.h"
#include "Tools/ToolUserSaveHelper.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "PropertyCustomizationHelpers.h"
#include "Dom/JsonObject.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Input/SCheckBox.h"


#define LOCTEXT_NAMESPACE "MaterialTransferTool"
 
void SMaterialTttributeTransfer::Construct(const FArguments& InArgs)
{
    SaveConfigFileName = TEXT("DefaultSettings");
    TargetSavePath = TEXT("/Game/"); 
 
    ChildSlot
    [
        SNew(SVerticalBox)
 
        // 1. 配置管理
        + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
        [
            SNew(SBorder).BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight().Padding(5)
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                    [ SNew(STextBlock).Text(LOCTEXT("ConfigLabel", "配置文件名: ")) ]
                    + SHorizontalBox::Slot().FillWidth(1.0f).Padding(5, 0)
                    [
                        SNew(SEditableTextBox)
                        .Text(FText::FromString(SaveConfigFileName))
                        .OnTextChanged_Lambda([this](const FText& InText) { SaveConfigFileName = InText.ToString(); })
                    ]
                    + SHorizontalBox::Slot().AutoWidth()
                    [
                        SNew(SButton).Text(LOCTEXT("SaveBtn", "保存"))
                        .OnClicked_Lambda([this]() { SaveSettings(); return FReply::Handled(); })
                        .ToolTipText(LOCTEXT("SaveBtnTip", "保存配置"))
                    ]
                    + SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 0, 0)
                    [
                        SNew(SButton).Text(LOCTEXT("LoadBtn", "加载"))
                        .OnClicked_Lambda([this]() { LoadSettings(); return FReply::Handled(); })
                        .ToolTipText(LOCTEXT("LoadBtnTip", "加载配置"))
                    ]
                    + SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 0, 0)
                    [
                        SNew(SButton).Text(LOCTEXT("OpenFolderBtn", "打开配置"))
                        .OnClicked_Lambda([this]() {
                            FString Dir = GetSaveDirectory();
                            // 确保目录已存在（首次点击可能尚未创建）
                            IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
                            if (!PF.DirectoryExists(*Dir)) PF.CreateDirectoryTree(*Dir);
                            // 转成 Windows 原生反斜杠路径，否则 ShellExecute 的 explore 动词对正斜杠路径会静默失败
                            FString NativeDir = FPaths::ConvertRelativePathToFull(Dir).Replace(TEXT("/"), TEXT("\\"));
                            FPlatformProcess::ExploreFolder(*NativeDir);
                            AppendLog(TEXT("已打开配置文件夹: ") + NativeDir);
                            return FReply::Handled();
                        })
                        .ToolTipText(LOCTEXT("OpenFolderBtnTip", "打开配置文件夹"))
                    ]
                ]
                + SVerticalBox::Slot().AutoHeight().Padding(5)
                [
                    SNew(STextBlock)
                    .AutoWrapText(true)
                    .Text(LOCTEXT("MaterialAttributeTransferHelper",
                        "使用方法：\n"
                        "  1. 选择需要继承自的哪个材质或材质实例类\n"
                        "  2. 内容浏览器中选择一个或多个需要被转移参数值的材质实例或材质类\n"
                        "  3. 点击 开始转移参数 后会生成材质并将被转换的材质参数赋值给以母材质为父类新生成的材质\n"
                        ))
                   
                ]
            ]
        ]
 
        // 2. 母材质选择器
        + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
            [ SNew(STextBlock).Text(LOCTEXT("TargetLabel", "目标母材质: ")).MinDesiredWidth(100) ]
            + SHorizontalBox::Slot().FillWidth(1.0f)
            [
                SNew(SObjectPropertyEntryBox)
                .AllowedClass(UMaterialInterface::StaticClass())
                .OnObjectChanged(this, &SMaterialTttributeTransfer::OnMasterMaterialChanged)
                .ObjectPath(this, &SMaterialTttributeTransfer::GetMasterMaterialPath)
                .DisplayThumbnail(true)
            ]
        ]
 
        // 3. 转移目标保存路径
        + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
        [
            SNew(SVerticalBox)
            // 3.1 路径输入行
            + SVerticalBox::Slot().AutoHeight()
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                [ SNew(STextBlock).Text(LOCTEXT("PathLabel", "生成保存路径: ")).MinDesiredWidth(100) ]
                + SHorizontalBox::Slot().FillWidth(1.0f)
                [
                    SNew(SEditableTextBox)
                    .HintText(LOCTEXT("PathHint", "例如 /Game/Materials/Generated"))
                    .Text_Lambda([this](){ return FText::FromString(TargetSavePath); })
                    .OnTextChanged_Lambda([this](const FText& T){ TargetSavePath = T.ToString(); })
                    .IsEnabled_Lambda([this](){ return !bSaveToRespectiveFolders; })
                ]
                + SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 0, 0)
                [
                    SNew(SButton).Text(LOCTEXT("GetPathBtn", "获取当前路径"))
                    .OnClicked_Lambda([this](){ UpdateCurrentPathFromContentBrowser(); return FReply::Handled(); })
                    .IsEnabled_Lambda([this](){ return !bSaveToRespectiveFolders; })
                ]
            ]
            // 3.2 保存到各自文件夹开关（默认打钩）
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4, 0, 0)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                [
                    SNew(SCheckBox)
                    .IsChecked_Lambda([this]() { return bSaveToRespectiveFolders ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { bSaveToRespectiveFolders = (NewState == ECheckBoxState::Checked); })
                ]
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(4, 0, 0, 0)
                [ SNew(STextBlock).Text(LOCTEXT("SaveToFolderTip", "保存到各自文件夹中")) ]
            ]
        ]
 
        
        + SVerticalBox::Slot().AutoHeight().Padding(10, 0, 10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight()
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                [
                    SNew(SCheckBox)
                    .IsChecked_Lambda([this]() { return bForceGenerateMaterial ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
                    {
                        bForceGenerateMaterial = (NewState == ECheckBoxState::Checked);
                        if (bForceGenerateMaterial) bForceGenerateInstance = false; 
                    })
                ]
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(4, 0, 20, 0)
                [ SNew(STextBlock).Text(LOCTEXT("GenMatChk", "转换后统一生成材质类")) ]
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
                [
                    SNew(SCheckBox)
                    .IsChecked_Lambda([this]() { return bForceGenerateInstance ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
                    {
                        bForceGenerateInstance = (NewState == ECheckBoxState::Checked);
                        if (bForceGenerateInstance) bForceGenerateMaterial = false; 
                    })
                    
                ]
                + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(4, 0, 0, 0)
                [ SNew(STextBlock).Text(LOCTEXT("GenInstChk", "转换后统一生成材质实例")) ]
            ]
            + SVerticalBox::Slot().AutoHeight()
            [
                SNew(STextBlock)
                .Text(LOCTEXT("CkeckTip", "两者都不勾选：按源材质类型生成（实例→实例，材质类→材质类）。"))
            ]
           
        ]

        // 4. 参数映射列表
        + SVerticalBox::Slot().FillHeight(0.6f).Padding(10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(1.f).Padding(2)[SNew(STextBlock).Text(LOCTEXT("TitleL", "母材质参数名 (新)"))]
                + SHorizontalBox::Slot().FillWidth(1.f).Padding(2)[SNew(STextBlock).Text(LOCTEXT("TitleR", "源材质变量名 (旧)"))]
            ]
            + SVerticalBox::Slot().FillHeight(1.0f)
            [
                SNew(SScrollBox)
                + SScrollBox::Slot() [ SAssignNew(MappingContainer, SVerticalBox) ]
            ]
        ]
 
        // 5. 日志窗口 (修复滚动问题)
        + SVerticalBox::Slot().FillHeight(0.3f).Padding(10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight()[ SNew(STextBlock).Text(LOCTEXT("LogTitle", "执行日志:")) ]
            + SVerticalBox::Slot().FillHeight(1.0f)
            [
                SNew(SBorder).BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
                [
                    SAssignNew(LogScrollBox, SScrollBox)
                    + SScrollBox::Slot()
                    [
                        SAssignNew(LogWindow, SMultiLineEditableText)
                        .IsReadOnly(true)
                        .Text_Lambda([this](){ return LogContent; })
                        .AutoWrapText(true)
                    ]
                ]
            ]
        ]
 
        // 6. 操作按钮
        + SVerticalBox::Slot().AutoHeight().Padding(10)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth()
            [
                SNew(SButton).Text(LOCTEXT("AddBtn", "添加参数行"))
                .OnClicked_Lambda([this]() { AddMappingRow(); return FReply::Handled(); })
            ]
            + SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
            [
                SNew(SButton).Text(LOCTEXT("RunBtn", "开始转移参数"))
                .ButtonStyle(FAppStyle::Get(), "PrimaryButton")
                .OnClicked(this, &SMaterialTttributeTransfer::OnExecuteTransfer)
                .IsEnabled_Lambda([this]() { return TargetMasterMaterial.IsValid(); })
            ]
        ]
    ];
 
    AddMappingRow();
}
 
void SMaterialTttributeTransfer::AppendLog(const FString& InLog)
{
    FString NewLine = FDateTime::Now().ToString(TEXT("[%H:%M:%S] ")) + InLog + TEXT("\n");
    LogContent = FText::FromString(LogContent.ToString() + NewLine);
    
    // 强制 UI 更新并在下一帧滚动到底部
    if (LogScrollBox.IsValid())
    {
        LogScrollBox->ScrollToEnd();
    }
}
 
void SMaterialTttributeTransfer::UpdateCurrentPathFromContentBrowser()
{
    FContentBrowserModule& CBModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    TArray<FString> SelectedPaths;
    CBModule.Get().GetSelectedPathViewFolders(SelectedPaths);
    
    if (SelectedPaths.Num() > 0)
    {
        FString Path = SelectedPaths[0];
        

        if (Path.StartsWith(TEXT("/All")))
        {
            Path.RemoveFromStart(TEXT("/All"));
        }
        
        // 如果裁剪后变空了（说明选中的是根目录），设为 /Game
        if (Path.IsEmpty() || Path == TEXT("/")) 
        {
            Path = TEXT("/Game");
        }
 
        TargetSavePath = Path;
        AppendLog(FString::Printf(TEXT("路径已获取并修正: %s"), *TargetSavePath));
    }
}
 
TSharedRef<SWidget> SMaterialTttributeTransfer::CreateMappingRowWidget(TSharedPtr<FParamMappingPair> InPair)
{
    return SNew(SHorizontalBox)
        + SHorizontalBox::Slot().FillWidth(1.0f).Padding(2)
        [
            SNew(SEditableTextBox)
            .Text_Lambda([InPair]() { return FText::FromString(InPair->TargetParamName); })
            .OnTextCommitted_Lambda([InPair](const FText& T, ETextCommit::Type) { InPair->TargetParamName = T.ToString(); })
        ]
        + SHorizontalBox::Slot().FillWidth(1.0f).Padding(2)
        [
            SNew(SEditableTextBox)
            .Text_Lambda([InPair]() { return FText::FromString(InPair->SourceParamName); })
            .OnTextCommitted_Lambda([InPair](const FText& T, ETextCommit::Type) { InPair->SourceParamName = T.ToString(); })
        ];
}
 
void SMaterialTttributeTransfer::SaveSettings()
{
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());
    RootObject->SetStringField(TEXT("MasterMaterial"), TargetMasterMaterial.IsValid() ? TargetMasterMaterial->GetPathName() : TEXT(""));
    RootObject->SetStringField(TEXT("TargetSavePath"), TargetSavePath);
    RootObject->SetBoolField(TEXT("SaveToRespectiveFolders"), bSaveToRespectiveFolders);
    RootObject->SetBoolField(TEXT("ForceGenerateMaterial"), bForceGenerateMaterial);
    RootObject->SetBoolField(TEXT("ForceGenerateInstance"), bForceGenerateInstance);
 
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    for (const auto& Pair : MappingList)
    {
        TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
        Obj->SetStringField(TEXT("T"), Pair->TargetParamName);
        Obj->SetStringField(TEXT("S"), Pair->SourceParamName);
        JsonArray.Add(MakeShareable(new FJsonValueObject(Obj)));
    }
    RootObject->SetArrayField(TEXT("Mappings"), JsonArray);
 
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    if (FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
    {
        FString FullPath = GetFullConfigPath();
        if (FFileHelper::SaveStringToFile(OutputString, *FullPath))
            AppendLog(TEXT("配置已保存: ") + FullPath);
    }
}
 
void SMaterialTttributeTransfer::LoadSettings()
{
    FString FullPath = GetFullConfigPath();
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FullPath)) { AppendLog(TEXT("找不到配置文件: ") + FullPath); return; }
 
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (FJsonSerializer::Deserialize(Reader, RootObject) && RootObject.IsValid())
    {
        FString MatPath = RootObject->GetStringField(TEXT("MasterMaterial"));
        TargetMasterMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MatPath));
        TargetSavePath = RootObject->GetStringField(TEXT("TargetSavePath"));
        if (TargetSavePath.IsEmpty()) TargetSavePath = TEXT("/Game/");

        if (RootObject->HasField(TEXT("SaveToRespectiveFolders")))
            bSaveToRespectiveFolders = RootObject->GetBoolField(TEXT("SaveToRespectiveFolders"));

        if (RootObject->HasField(TEXT("ForceGenerateMaterial")))
            bForceGenerateMaterial = RootObject->GetBoolField(TEXT("ForceGenerateMaterial"));
        if (RootObject->HasField(TEXT("ForceGenerateInstance")))
            bForceGenerateInstance = RootObject->GetBoolField(TEXT("ForceGenerateInstance"));
 
        MappingList.Empty();
        const TArray<TSharedPtr<FJsonValue>>* JsonArray;
        if (RootObject->TryGetArrayField(TEXT("Mappings"), JsonArray))
        {
            for (const auto& Val : *JsonArray)
            {
                TSharedPtr<FJsonObject> Obj = Val->AsObject();
                TSharedPtr<FParamMappingPair> NewPair = MakeShared<FParamMappingPair>();
                NewPair->TargetParamName = Obj->GetStringField(TEXT("T"));
                NewPair->SourceParamName = Obj->GetStringField(TEXT("S"));
                MappingList.Add(NewPair);
            }
        }
        RefreshMappingUI();
        AppendLog(TEXT("配置加载成功."));
    }
}
 
	FReply SMaterialTttributeTransfer::OnExecuteTransfer()
{
    // 1. 【核心修复】直接从内容浏览器获取选中的资产数据
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    TArray<FAssetData> SelectedAssetsData;
    ContentBrowserModule.Get().GetSelectedAssets(SelectedAssetsData);
 
    // 过滤出材质类资产
    TArray<UMaterialInterface*> SelectedMaterials;
    for (const FAssetData& AssetData : SelectedAssetsData)
    {
        if (UMaterialInterface* Mat = Cast<UMaterialInterface>(AssetData.GetAsset()))
        {
            SelectedMaterials.Add(Mat);
        }
    }
 
    if (SelectedMaterials.Num() == 0)
    {
        AppendLog(TEXT("错误: 未在内容浏览器选中任何有效的材质或材质实例！"));
        return FReply::Handled();
    }
 
    if (!TargetMasterMaterial.IsValid())
    {
        AppendLog(TEXT("错误: 请先在工具上方选择目标母材质！"));
        return FReply::Handled();
    }
 
    // 2. 保存路径模式 & 输出类型
    const bool bSaveToRespective = bSaveToRespectiveFolders;

    FString OutputModeStr;
    if (bForceGenerateInstance) OutputModeStr = TEXT("统一材质实例");
    else if (bForceGenerateMaterial) OutputModeStr = TEXT("统一材质类");
    else OutputModeStr = TEXT("按源材质类型");

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AppendLog(FString::Printf(
        TEXT("开始处理... 选中数量: %d, 保存模式: %s, 输出类型: %s"),
        SelectedMaterials.Num(),
        bSaveToRespective ? TEXT("各自源文件夹") : *TargetSavePath,
        *OutputModeStr));
 
    int32 SuccessCount = 0;
    int32 SkipCount = 0;
 
    // 3. 循环处理
    for (UMaterialInterface* SourceMat : SelectedMaterials)
    {
        // 排除母材质本身
        if (SourceMat == TargetMasterMaterial.Get())
        {
            AppendLog(FString::Printf(TEXT("跳过: %s (它是母材质本身)"), *SourceMat->GetName()));
            SkipCount++;
            continue;
        }

        // 计算本次生成的目标路径
        FString FinalPath = TargetSavePath.TrimStartAndEnd();
        if (bSaveToRespective)
        {
            // 使用源材质所在的包路径，生成的 MIC 保存到各自的文件夹中
            FinalPath = FPaths::GetPath(SourceMat->GetPathName());
        }
        else
        {
            if (FinalPath.StartsWith(TEXT("/All")))
            {
                FinalPath.RemoveFromStart(TEXT("/All"));
            }

            // 统一处理斜杠
            while (FinalPath.StartsWith(TEXT("/"))) { FinalPath.RemoveFromStart(TEXT("/")); }
            while (FinalPath.EndsWith(TEXT("/"))) { FinalPath.RemoveFromEnd(TEXT("/")); }

            if (FinalPath.StartsWith(TEXT("Game")))
            {
                FinalPath = TEXT("/") + FinalPath;
            }
            else
            {
                FinalPath = FinalPath.IsEmpty() ? TEXT("/Game") : TEXT("/Game/") + FinalPath;
            }
        }

        // 确保路径中没有 //
        FinalPath.ReplaceInline(TEXT("//"), TEXT("/"));

        // 决定本次输出类型
        enum class EOutputType { MatchSource, Instance, Material };
        EOutputType OutType = bForceGenerateInstance ? EOutputType::Instance
            : (bForceGenerateMaterial ? EOutputType::Material : EOutputType::MatchSource);
        if (OutType == EOutputType::MatchSource)
        {
            // 按源材质类型：材质实例 → 实例；材质类 → 材质类
            OutType = Cast<UMaterialInstanceConstant>(SourceMat) ? EOutputType::Instance : EOutputType::Material;
        }

        if (OutType == EOutputType::Instance)
        {
            // 统一/匹配生成材质实例
            FString NewAssetName = SourceMat->GetName() + TEXT("_INST");
            MakeUniqueAssetName(FinalPath, NewAssetName, NewAssetName);

            UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
            UObject* NewAsset = AssetTools.CreateAsset(NewAssetName, FinalPath, UMaterialInstanceConstant::StaticClass(), Factory);
            UMaterialInstanceConstant* NewMIC = Cast<UMaterialInstanceConstant>(NewAsset);

            if (NewMIC)
            {
                NewMIC->SetParentEditorOnly(TargetMasterMaterial.Get());
                ApplyParameterValues(NewMIC, SourceMat);
                NewMIC->PostEditChange();
                FAssetRegistryModule::AssetCreated(NewMIC);

                SuccessCount++;
                AppendLog(FString::Printf(TEXT("成功生成(实例): %s -> %s"), *NewAssetName, *FinalPath));
            }
            else
            {
                AppendLog(FString::Printf(TEXT("失败: 无法在路径 %s 下创建资产 %s"), *FinalPath, *NewAssetName));
            }
        }
        else
        {
            // 统一/匹配生成材质类：复制母材质（基础材质）并烘焙参数值
            UMaterial* Template = FindBaseMaterialTemplate(TargetMasterMaterial.Get());
            if (!Template)
            {
                AppendLog(FString::Printf(
                    TEXT("跳过: %s (统一生成材质类需要母材质为材质类，或能向上追溯到材质类)"), *SourceMat->GetName()));
                SkipCount++;
                continue;
            }

            FString NewAssetName = SourceMat->GetName() + TEXT("_MAT");
            MakeUniqueAssetName(FinalPath, NewAssetName, NewAssetName);

            FString PackagePath = FinalPath / NewAssetName;
            UPackage* Pkg = CreatePackage(*PackagePath);
            UMaterial* NewMat = DuplicateObject<UMaterial>(Template, Pkg, *NewAssetName);
            if (NewMat)
            {
                NewMat->SetFlags(RF_Public | RF_Standalone);
                ApplyParameterValues(NewMat, SourceMat);
                NewMat->PostEditChange();
                NewMat->MarkPackageDirty();
                FAssetRegistryModule::AssetCreated(NewMat);

                SuccessCount++;
                AppendLog(FString::Printf(TEXT("成功生成(材质类): %s -> %s"), *NewAssetName, *FinalPath));
            }
            else
            {
                AppendLog(FString::Printf(TEXT("失败: 无法在路径 %s 下复制材质 %s"), *FinalPath, *NewAssetName));
            }
        }
    }
 
    AppendLog(FString::Printf(TEXT("任务完成！成功: %d, 跳过: %d"), SuccessCount, SkipCount));
    UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
 
    return FReply::Handled();
}
 
UMaterial* SMaterialTttributeTransfer::FindBaseMaterialTemplate(UMaterialInterface* InMat) const
{
    UMaterialInterface* Cur = InMat;
    while (Cur)
    {
        if (UMaterial* Mat = Cast<UMaterial>(Cur))
        {
            return Mat;
        }
        // 材质实例可向上追溯到其父材质
        UMaterialInstance* MI = Cast<UMaterialInstance>(Cur);
        Cur = MI ? MI->Parent.Get() : nullptr;
    }
    return nullptr;
}

void SMaterialTttributeTransfer::MakeUniqueAssetName(const FString& PackagePath, const FString& BaseName, FString& OutName) const
{
    OutName = BaseName;
    int32 Suffix = 1;
    bool bExists = false;
    do
    {
        FString Full = PackagePath / OutName;
        bExists = (FindObject<UPackage>(nullptr, *Full) != nullptr)
               || FPackageName::DoesPackageExist(Full);
        if (bExists)
        {
            OutName = FString::Printf(TEXT("%s_%d"), *BaseName, Suffix++);
        }
    } while (bExists);
}

void SMaterialTttributeTransfer::ApplyParameterValues(UMaterialInterface* Target, UMaterialInterface* Source) const
{
    for (const TSharedPtr<FParamMappingPair>& Mapping : MappingList)
    {
        if (Mapping->TargetParamName.IsEmpty() || Mapping->SourceParamName.IsEmpty()) continue;

        FName DestName(*Mapping->TargetParamName);
        FName SrcName(*Mapping->SourceParamName);

        // 转移 Texture
        UTexture* SourceTex = nullptr;
        if (Source->GetTextureParameterValue(SrcName, SourceTex))
        {
            if (UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(Target)) MIC->SetTextureParameterValueEditorOnly(DestName, SourceTex);
            else if (UMaterial* Mat = Cast<UMaterial>(Target)) Mat->SetTextureParameterValueEditorOnly(DestName, SourceTex);
        }

        // 转移 Scalar
        float SourceScalar = 0.f;
        if (Source->GetScalarParameterValue(SrcName, SourceScalar))
        {
            if (UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(Target)) MIC->SetScalarParameterValueEditorOnly(DestName, SourceScalar);
            else if (UMaterial* Mat = Cast<UMaterial>(Target)) Mat->SetScalarParameterValueEditorOnly(DestName, SourceScalar);
        }

        // 转移 Vector
        FLinearColor SourceVector;
        if (Source->GetVectorParameterValue(SrcName, SourceVector))
        {
            if (UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(Target)) MIC->SetVectorParameterValueEditorOnly(DestName, SourceVector);
            else if (UMaterial* Mat = Cast<UMaterial>(Target)) Mat->SetVectorParameterValueEditorOnly(DestName, SourceVector);
        }
    }
}

FString SMaterialTttributeTransfer::GetSaveDirectory() const {
    // 每个工具在自己的子目录里存文件，互不干扰；目录不存在会被自动建好
    return FToolUserSave::GetToolSaveDir(TEXT("MaterialAttributeTransfer"));
}
 
FString SMaterialTttributeTransfer::GetFullConfigPath() const {
    FString FileName = SaveConfigFileName;
    if (!FileName.EndsWith(TEXT(".json"))) FileName += TEXT(".json");
    // 升级兼容：把老版本直接放在根目录的配置文件搬到新的工具子目录
    FToolUserSave::MigrateLegacyFile(TEXT("MaterialAttributeTransfer"), FileName);
    return GetSaveDirectory() + FileName;
}
 
void SMaterialTttributeTransfer::RefreshMappingUI() {
    if (MappingContainer.IsValid()) {
        MappingContainer->ClearChildren();
        for (auto& P : MappingList) MappingContainer->AddSlot().AutoHeight()[CreateMappingRowWidget(P)];
    }
}
 
void SMaterialTttributeTransfer::AddMappingRow() {
    TSharedPtr<FParamMappingPair> NP = MakeShared<FParamMappingPair>();
    MappingList.Add(NP);
    if (MappingContainer.IsValid()) MappingContainer->AddSlot().AutoHeight()[CreateMappingRowWidget(NP)];
}
 
void SMaterialTttributeTransfer::OnMasterMaterialChanged(const FAssetData& AssetData) { TargetMasterMaterial = Cast<UMaterialInterface>(AssetData.GetAsset()); }
FString SMaterialTttributeTransfer::GetMasterMaterialPath() const { return TargetMasterMaterial.IsValid() ? TargetMasterMaterial->GetPathName() : TEXT(""); }
 
#undef LOCTEXT_NAMESPACE