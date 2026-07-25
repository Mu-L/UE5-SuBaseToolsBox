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
    SaveConfigFileName = TEXT("MaterialAttributeTransfer_Data.json");
    TargetSavePath = TEXT("/Game/"); 
 
    ChildSlot
    [
        SNew(SVerticalBox)
 
        // 1. 配置文件管理 (保存/加载 JSON)
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
 
        // 3. 转移目标保存路径 (修复了 SAssignNew(nullptr) 错误)
        + SVerticalBox::Slot().AutoHeight().Padding(10, 5)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
            [ SNew(STextBlock).Text(LOCTEXT("PathLabel", "生成保存路径: ")).MinDesiredWidth(100) ]
            + SHorizontalBox::Slot().FillWidth(1.0f)
            [
                // 这里改成了 SNew，因为不需要暴露该组件给变量
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
 
        // 5. 日志窗口
        + SVerticalBox::Slot().FillHeight(0.3f).Padding(10, 5)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight()[ SNew(STextBlock).Text(LOCTEXT("LogTitle", "执行日志:")) ]
            + SVerticalBox::Slot().FillHeight(1.0f)
            [
                SNew(SBorder).BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
                [
                    SAssignNew(LogWindow, SMultiLineEditableText)
                    .IsReadOnly(true)
                    .Text_Lambda([this](){ return LogContent; })
                    .AutoWrapText(true)
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
    if (LogWindow.IsValid()) { LogWindow->ScrollTo(FTextLocation(LogContent.ToString().Len())); }
}
 
void SMaterialTttributeTransfer::UpdateCurrentPathFromContentBrowser()
{
    FContentBrowserModule& CBModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    TArray<FString> SelectedPaths;
    CBModule.Get().GetSelectedPathViewFolders(SelectedPaths);
    if (SelectedPaths.Num() > 0)
    {
        TargetSavePath = SelectedPaths[0];
        AppendLog(FString::Printf(TEXT("路径已同步: %s"), *TargetSavePath));
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
    TArray<UObject*> SelectedAssets;
    GEditor->GetSelectedObjects()->GetSelectedObjects(UMaterialInterface::StaticClass(), SelectedAssets);
 
    if (SelectedAssets.Num() == 0) { AppendLog(TEXT("错误: 未选中任何源材质资产！")); return FReply::Handled(); }
 
    FString FinalPath = TargetSavePath.TrimStartAndEnd();
    if (!FinalPath.StartsWith(TEXT("/Game")))
    {
        FString CleanedPath = FinalPath.Replace(TEXT("/Game"), TEXT(""));
        CleanedPath.TrimStartAndEndInline();
        CleanedPath.RemoveFromStart(TEXT("/"));
        CleanedPath.RemoveFromEnd(TEXT("/"));
        FinalPath = TEXT("/Game/") + CleanedPath;
    }
 
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AppendLog(FString::Printf(TEXT("开始批量处理到路径: %s"), *FinalPath));
 
    int32 Count = 0;
    for (UObject* Asset : SelectedAssets)
    {
        UMaterialInterface* Src = Cast<UMaterialInterface>(Asset);
        if (!Src || Src == TargetMasterMaterial.Get()) continue;
 
        FString NewName = Src->GetName() + "_INST";
        UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
        UObject* NewAsset = AssetTools.CreateAsset(NewName, FinalPath, UMaterialInstanceConstant::StaticClass(), Factory);
        UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(NewAsset);
 
        if (MIC)
        {
            MIC->SetParentEditorOnly(TargetMasterMaterial.Get());
            for (auto& M : MappingList)
            {
                if (M->TargetParamName.IsEmpty()) continue;
                FName DN(*M->TargetParamName); FName SN(*M->SourceParamName);
                UTexture* T = nullptr; if (Src->GetTextureParameterValue(SN, T)) MIC->SetTextureParameterValueEditorOnly(DN, T);
                float S = 0.f; if (Src->GetScalarParameterValue(SN, S)) MIC->SetScalarParameterValueEditorOnly(DN, S);
                FLinearColor V; if (Src->GetVectorParameterValue(SN, V)) MIC->SetVectorParameterValueEditorOnly(DN, V);
            }
            MIC->PostEditChange();
            FAssetRegistryModule::AssetCreated(MIC);
            Count++;
            AppendLog(TEXT("生成成功: ") + MIC->GetName());
        }
    }
 
    AppendLog(FString::Printf(TEXT("任务完成，共生成 %d 个材质实例。"), Count));
    UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
    return FReply::Handled();
}
 
FString SMaterialTttributeTransfer::GetSaveDirectory() const {
    FString Dir = FPaths::ProjectPluginsDir() + TEXT("ToolsBox/Source/ToolsBox/Public/Tools/ToolUserDataSave/");
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    if (!PF.DirectoryExists(*Dir)) PF.CreateDirectoryTree(*Dir);
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