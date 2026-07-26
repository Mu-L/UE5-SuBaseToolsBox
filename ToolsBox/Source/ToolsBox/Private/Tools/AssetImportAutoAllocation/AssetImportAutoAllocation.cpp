#include "Tools/AssetImportAutoAllocation/AssetImportAutoAllocation.h"
 
// 关键头文件：解决 StaticClass()、FAssetData 和 Slate 组件找不到的问题
#include "UObject/Class.h"           // 必须：识别 UClass::StaticClass()
#include "AssetRegistry/AssetData.h" // 必须：识别 FAssetData
#include "PropertyCustomizationHelpers.h" // 必须：识别 SObjectPropertyEntryBox
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
 
#define LOCTEXT_NAMESPACE "AssetImportTool"
 
void SAssetImportAutoAllocation::Construct(const FArguments& InArgs)
{
    // 初始化一行默认映射
    MappingItems.Add(MakeShareable(new FAssetTypeMapping()));
 
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(10)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Title", "资产自动分类规则配置 (UE 5.8兼容版)"))
            .Font(FAppStyle::GetFontStyle("BoldFont"))
        ]
        + SVerticalBox::Slot().FillHeight(1.0f).Padding(5)
        [
            SNew(SScrollBox)
            + SScrollBox::Slot()
            [
                SAssignNew(ListContainer, SVerticalBox)
            ]
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(10)
        [
            SNew(SButton)
            .Text(LOCTEXT("AddBtn", "+ 添加新映射规则"))
            .OnClicked_Lambda([this]() {
                MappingItems.Add(MakeShareable(new FAssetTypeMapping()));
                RefreshMappingList();
                return FReply::Handled();
            })
        ]
    ];
 
    RefreshMappingList();
}
 
void SAssetImportAutoAllocation::RefreshMappingList()
{
    if (!ListContainer.IsValid()) return;
 
    ListContainer->ClearChildren();
 
    for (const TSharedPtr<FAssetTypeMapping>& Item : MappingItems)
    {
        ListContainer->AddSlot()
        .AutoHeight()
        .Padding(0.0f, 2.0f)
        [
            CreateRowWidget(Item)
        ];
    }
}
 
TSharedRef<SWidget> SAssetImportAutoAllocation::CreateRowWidget(TSharedPtr<FAssetTypeMapping> Item)
{
    return SNew(SHorizontalBox)
        
        // 1. 资产类选择 (左侧)
        + SHorizontalBox::Slot()
        .FillWidth(0.5f)
        .Padding(5, 0)
        [
            SNew(SObjectPropertyEntryBox)
            // 现代版本 API 使用单数形式
            .AllowedClass(UClass::StaticClass()) 
            // 实时显示当前路径
            .ObjectPath_Lambda([Item]() { 
                return Item->AssetClass.IsValid() ? Item->AssetClass->GetPathName() : FString(); 
            })
            // 使用标准的 OnObjectChanged_Lambda 委托
            .OnObjectChanged_Lambda([Item](const FAssetData& AssetData) {
                // 当用户从选择器选中一个“类”资产时
                Item->AssetClass = Cast<UClass>(AssetData.GetAsset());
            })
        ]
 
        // 2. 存放文件夹路径 (右侧)
        + SHorizontalBox::Slot()
        .FillWidth(0.5f)
        .Padding(5, 0)
        [
            SNew(SEditableTextBox)
            .HintText(LOCTEXT("PathHint", "目标文件夹，例如 /Game/Art/"))
            .Text_Lambda([Item]() { return FText::FromString(Item->TargetPath); })
            .OnTextCommitted_Lambda([Item](const FText& NewText, ETextCommit::Type CommitType) {
                Item->TargetPath = NewText.ToString();
            })
        ]
 
        // 3. 删除按钮 (消除规则)
        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(SButton)
            .ButtonStyle(FAppStyle::Get(), "SimpleButton")
            .OnClicked_Lambda([this, Item]() {
                // 彻底从映射列表中“消除”该配置
                MappingItems.Remove(Item);
                RefreshMappingList(); // 立即刷新界面
                return FReply::Handled();
            })
            [
                SNew(STextBlock)
                .Text(FText::FromString(" X "))
                .ColorAndOpacity(FLinearColor::Red)
            ]
        ];
}
 
#undef LOCTEXT_NAMESPACE