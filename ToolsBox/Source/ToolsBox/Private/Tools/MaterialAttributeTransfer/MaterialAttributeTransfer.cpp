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
#include "Tools/MaterialTttributeTransfer/MaterialTttributeTransfer.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "PropertyCustomizationHelpers.h"
#include "Dom/JsonObject.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Widgets/Text/SMultiLineEditableText.h"


#define LOCTEXT_NAMESPACE "MaterialTransferTool"
 
void SMaterialTttributeTransfer::Construct(const FArguments& InArgs)
{
    SaveConfigFileName = TEXT("DefaultSettings.json");
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
                        SNew(SButton).Text(LOCTEXT("SaveBtn", "保存配置"))
                        .OnClicked_Lambda([this]() { SaveSettings(); return FReply::Handled(); })
                    ]
                    + SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 0, 0)
                    [
                        SNew(SButton).Text(LOCTEXT("LoadBtn", "加载配置"))
                        .OnClicked_Lambda([this]() { LoadSettings(); return FReply::Handled(); })
                    ]
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
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
            [ SNew(STextBlock).Text(LOCTEXT("PathLabel", "生成保存路径: ")).MinDesiredWidth(100) ]
            + SHorizontalBox::Slot().FillWidth(1.0f)
            [
                SNew(SEditableTextBox)
                .HintText(LOCTEXT("PathHint", "例如 /Game/Materials/Generated"))
                .Text_Lambda([this](){ return FText::FromString(TargetSavePath); })
                .OnTextChanged_Lambda([this](const FText& T){ TargetSavePath = T.ToString(); })
            ]
            + SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 0, 0)
            [
                SNew(SButton).Text(LOCTEXT("GetPathBtn", "获取当前路径"))
                .OnClicked_Lambda([this](){ UpdateCurrentPathFromContentBrowser(); return FReply::Handled(); })
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
 
    // 2. 路径清理逻辑
    FString FinalPath = TargetSavePath.TrimStartAndEnd();
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
    
    // 确保路径中没有 //
    FinalPath.ReplaceInline(TEXT("//"), TEXT("/"));
 
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AppendLog(FString::Printf(TEXT("开始处理... 选中数量: %d, 目标路径: %s"), SelectedMaterials.Num(), *FinalPath));
 
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
 
        // 创建 MIC
        FString NewAssetName = SourceMat->GetName() + TEXT("_INST");
        UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
        
        UObject* NewAsset = AssetTools.CreateAsset(NewAssetName, FinalPath, UMaterialInstanceConstant::StaticClass(), Factory);
        UMaterialInstanceConstant* NewMIC = Cast<UMaterialInstanceConstant>(NewAsset);
 
        if (NewMIC)
        {
            NewMIC->SetParentEditorOnly(TargetMasterMaterial.Get());
 
            for (const TSharedPtr<FParamMappingPair>& Mapping : MappingList)
            {
                if (Mapping->TargetParamName.IsEmpty() || Mapping->SourceParamName.IsEmpty()) continue;
 
                FName DestName(*Mapping->TargetParamName);
                FName SrcName(*Mapping->SourceParamName);
 
                // 转移 Texture
                UTexture* SourceTex = nullptr;
                if (SourceMat->GetTextureParameterValue(SrcName, SourceTex))
                    NewMIC->SetTextureParameterValueEditorOnly(DestName, SourceTex);
 
                // 转移 Scalar
                float SourceScalar = 0.f;
                if (SourceMat->GetScalarParameterValue(SrcName, SourceScalar))
                    NewMIC->SetScalarParameterValueEditorOnly(DestName, SourceScalar);
 
                // 转移 Vector
                FLinearColor SourceVector;
                if (SourceMat->GetVectorParameterValue(SrcName, SourceVector))
                    NewMIC->SetVectorParameterValueEditorOnly(DestName, SourceVector);
            }
 
            NewMIC->PostEditChange();
            FAssetRegistryModule::AssetCreated(NewMIC);
            
            SuccessCount++;
            AppendLog(FString::Printf(TEXT("成功生成: %s"), *NewAssetName));
        }
        else
        {
            AppendLog(FString::Printf(TEXT("失败: 无法在路径 %s 下创建资产 %s"), *FinalPath, *NewAssetName));
        }
    }
 
    AppendLog(FString::Printf(TEXT("任务完成！成功: %d, 跳过: %d"), SuccessCount, SkipCount));
    UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
 
    return FReply::Handled();
}
 
FString SMaterialTttributeTransfer::GetSaveDirectory() const {
    FString Dir = FPaths::ProjectPluginsDir() + TEXT("ToolsBox/Source/ToolsBox/Public/Tools/ToolUserDataSave/");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*Dir)) PlatformFile.CreateDirectoryTree(*Dir);
    return Dir;
}
 
FString SMaterialTttributeTransfer::GetFullConfigPath() const {
    FString FileName = SaveConfigFileName;
    if (!FileName.EndsWith(TEXT(".json"))) FileName += TEXT(".json");
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