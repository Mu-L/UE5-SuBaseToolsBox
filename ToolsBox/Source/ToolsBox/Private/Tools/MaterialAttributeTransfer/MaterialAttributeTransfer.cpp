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


#define LOCTEXT_NAMESPACE "MaterialTransferTool"
 
void SMaterialTttributeTransfer::Construct(const FArguments& InArgs)
{
    // 初始化时添加一行
    AddMappingRow();
 
    ChildSlot
    [
        SNew(SVerticalBox)
        
        // 1. 带图标的正方形母材质选择器
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock).Text(LOCTEXT("TargetLabel", "目标母材质: ")).MinDesiredWidth(100)
            ]
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            [
                SNew(SObjectPropertyEntryBox)
                .AllowedClass(UMaterialInterface::StaticClass())
                .OnObjectChanged(this, &SMaterialTttributeTransfer::OnMasterMaterialChanged)
                .ObjectPath(this, &SMaterialTttributeTransfer::GetMasterMaterialPath)
                .DisplayThumbnail(true) // 开启正方形图标预览
                .ThumbnailPool(nullptr)
            ]
        ]
 
        // 2. 映射列表标题
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10, 5)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().FillWidth(1.f).Padding(2)[SNew(STextBlock).Text(LOCTEXT("TitleL", "母材质参数名 (新)"))]
            + SHorizontalBox::Slot().FillWidth(1.f).Padding(2)[SNew(STextBlock).Text(LOCTEXT("TitleR", "源材质变量名 (旧)"))]
        ]
 
        // 3. 动态列表容器
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SAssignNew(MappingContainer, SVerticalBox)
        ]
 
        // 4. 底部控制栏
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .Text(LOCTEXT("AddBtn", "添加参数映射"))
                .OnClicked_Lambda([this](){ AddMappingRow(); return FReply::Handled(); })
            ]
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            .HAlign(HAlign_Right)
            [
                SNew(SButton)
                .Text(LOCTEXT("RunBtn", "开始转移参数"))
                .ButtonStyle(FAppStyle::Get(), "PrimaryButton")
                .OnClicked(this, &SMaterialTttributeTransfer::OnExecuteTransfer)
                .IsEnabled_Lambda([this](){ return TargetMasterMaterial.IsValid(); })
            ]
        ]
    ];
 
    // 填充初始 UI
    for (auto& Pair : MappingList)
    {
        MappingContainer->AddSlot().AutoHeight()[CreateMappingRowWidget(Pair)];
    }
}
 
void SMaterialTttributeTransfer::AddMappingRow()
{
    TSharedPtr<FParamMappingPair> NewPair = MakeShared<FParamMappingPair>();
    MappingList.Add(NewPair);
    if (MappingContainer.IsValid())
    {
        MappingContainer->AddSlot().AutoHeight()[CreateMappingRowWidget(NewPair)];
    }
}
 
TSharedRef<SWidget> SMaterialTttributeTransfer::CreateMappingRowWidget(TSharedPtr<FParamMappingPair> InPair)
{
    return SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .FillWidth(1.0f)
        .Padding(2)
        [
            SNew(SEditableTextBox)
            .HintText(LOCTEXT("HintL", "母材质参数: 如 BaseColor"))
            .OnTextCommitted_Lambda([InPair](const FText& T, ETextCommit::Type){ InPair->TargetParamName = T.ToString(); })
        ]
        + SHorizontalBox::Slot()
        .FillWidth(1.0f)
        .Padding(2)
        [
            SNew(SEditableTextBox)
            .HintText(LOCTEXT("HintR", "源材质参数: 如 AAA"))
            .OnTextCommitted_Lambda([InPair](const FText& T, ETextCommit::Type){ InPair->SourceParamName = T.ToString(); })
        ];
}
 
FReply SMaterialTttributeTransfer::OnExecuteTransfer()
{
    // 获取选中的材质资产
    TArray<UObject*> SelectedAssets;
    GEditor->GetSelectedObjects()->GetSelectedObjects(UMaterialInterface::StaticClass(), SelectedAssets);
 
    if (SelectedAssets.Num() == 0) return FReply::Handled();
 
    FString CurrentPath = GetCurrentContentBrowserPath();
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
 
    for (UObject* Asset : SelectedAssets)
    {
        UMaterialInterface* SourceMat = Cast<UMaterialInterface>(Asset);
        if (!SourceMat) continue;
 
        // 创建新的材质实例 (MIC)
        FString NewName = SourceMat->GetName() + "_Transferred";
        UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
        UMaterialInstanceConstant* NewMIC = Cast<UMaterialInstanceConstant>(
            AssetTools.CreateAsset(NewName, CurrentPath, UMaterialInstanceConstant::StaticClass(), Factory)
        );
 
        if (NewMIC)
        {
            NewMIC->SetParentEditorOnly(TargetMasterMaterial.Get());
 
            for (auto& Mapping : MappingList)
            {
                FName DestName(*Mapping->TargetParamName);
                FName SrcName(*Mapping->SourceParamName);
 
                // 1. 贴图转移
                UTexture* SourceTex = nullptr;
                if (SourceMat->GetTextureParameterValue(SrcName, SourceTex))
                {
                    NewMIC->SetTextureParameterValueEditorOnly(DestName, SourceTex);
                }
 
                // 2. 标量 (Float) 转移
                float SourceScalar = 0.f;
                if (SourceMat->GetScalarParameterValue(SrcName, SourceScalar))
                {
                    NewMIC->SetScalarParameterValueEditorOnly(DestName, SourceScalar);
                }
 
                // 3. 向量 (Vector/Color) 转移
                FLinearColor SourceVector;
                if (SourceMat->GetVectorParameterValue(SrcName, SourceVector))
                {
                    NewMIC->SetVectorParameterValueEditorOnly(DestName, SourceVector);
                }
 
                // 4. 布尔 (Static Switch) 转移
                bool SourceBool = false;
                FGuid TempGuid;
                if (SourceMat->GetStaticSwitchParameterValue(SrcName, SourceBool, TempGuid))
                {
                    // 注意：StaticSwitch 需要特殊处理，通常需要组件层级更新，此处简写：
                    // 在 Editor 环境下 MIC 设置 Switch 会触发重编译
                }
            }
 
            NewMIC->PostEditChange();
            FAssetRegistryModule::AssetCreated(NewMIC);
        }
    }
 
    // 强制保存新生成的资产
    UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
 
    return FReply::Handled();
}
 
FString SMaterialTttributeTransfer::GetCurrentContentBrowserPath()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    TArray<FString> SelectedPaths;
    ContentBrowserModule.Get().GetSelectedPathViewFolders(SelectedPaths);
    return SelectedPaths.Num() > 0 ? SelectedPaths[0] : TEXT("/Game");
}
 
void SMaterialTttributeTransfer::OnMasterMaterialChanged(const FAssetData& AssetData)
{
    TargetMasterMaterial = Cast<UMaterialInterface>(AssetData.GetAsset());
}
 
FString SMaterialTttributeTransfer::GetMasterMaterialPath() const
{
    return TargetMasterMaterial.IsValid() ? TargetMasterMaterial->GetPathName() : TEXT("");
}
 
#undef LOCTEXT_NAMESPACE
