#include "Slate_Assist/SlateAssistBuildFunctionLibrary.h"

#include "Components/VerticalBox.h"
#include "Framework/Docking/TabManager.h"
#include "Slate_Assist/FIconStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<SWidget> SlateAssistBuildFunctionLibrary::MakeToolBlock(const FText& ToolName, const FText& Description,
                                                                   const FName& IconName, const FName& TabID)
{
    return SNew(SBox)
        .Padding(FMargin(5.0f, 2.0f))
        [
            SNew(SBorder)
            .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder")) // 使用编辑器面板边框
            .Padding(10.0f)
            [
                SNew(SHorizontalBox)
 
                // 1. 左侧：图标
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(0, 0, 15.0f, 0)
                [
                    SNew(SImage)
                    .Image(FIconStyle::Get_Images().GetBrush(IconName))
                    .DesiredSizeOverride(FVector2D(64.0f, 64.0f)) // 缩减图标大小以适应横条
                ]
 
                // 2. 中间：文本区域（名字在上，描述在下）
                + SHorizontalBox::Slot()
                .FillWidth(1.0f) // 占据剩余所有空间
                .VAlign(VAlign_Center)
                [
                    SNew(SVerticalBox)
 
                    // 工具名
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0, 0, 0, 4.0f)
                    [
                        SNew(STextBlock)
                        .Text(ToolName)
                        .Font(FAppStyle::GetFontStyle("NormalFontBold"))
                    ]
 
                    // 描述
                    + SVerticalBox::Slot()
                    .AutoHeight()
                    [
                        SNew(STextBlock)
                        .Text(Description)
                        .AutoWrapText(true)
                        .Font(FAppStyle::GetFontStyle("NormalFont"))
                        .ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f))
                    ]
                ]
 
                // 3. 右侧：操作按钮
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(10.0f, 0, 0, 0)
                [
                    SNew(SBox)
                    .WidthOverride(100.0f)
                    .HeightOverride(40.0f)
                    [
                        SNew(SButton)
                        .HAlign(HAlign_Center)
                        .VAlign(VAlign_Center)
                        .ButtonStyle(FAppStyle::Get(), "PrimaryButton") // 使用醒目的主要按钮样式
                        .OnClicked_Lambda([TabID]()
                        {
                            FGlobalTabmanager::Get()->TryInvokeTab(FTabId(TabID));
                            return FReply::Handled();
                        })
                        [
                            SNew(STextBlock)
                            .Text(NSLOCTEXT("ToolsBox", "OpenBtn", "打开工具"))
                            .Font(FAppStyle::GetFontStyle("NormalFontBold"))
                        ]
                    ]
                ]
            ]
        ];
}