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
		.WidthOverride(300.0f) // 限制宽度，防止工具块横向拉得太长
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
 
			// 1. 顶部：工具缩略图标
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center) // 居中显示图标
			.Padding(5.0f)
			[
				SNew(SImage)
				// 从你的化妆间里取图，注意这里用了之前定义的 Plugin_Icon 集合
				.Image(FIconStyle::Get_Images().GetBrush(IconName))
				.DesiredSizeOverride(FVector2D(128.0f, 128.0f)) // 设置图标大小
			]
 
			// 2. 图标下面：工具名字
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(2.0f)
			[
				SNew(STextBlock)
				.Text(ToolName)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) // 字体加粗
				.ColorAndOpacity(FLinearColor::White)
			]
 
			// 3. 名字下面：描述
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5.0f, 10.0f)
			[
				SNew(STextBlock)
				.Text(Description)
				.AutoWrapText(true) // 【核心】自动换行，防止描述太长超出边界
				.Justification(ETextJustify::Center) // 文字居中
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f)) // 浅灰色描述
				.ToolTipText(Description) 
			]
 
			// 4. 底部：打开按钮
			+ SVerticalBox::Slot()
			.AutoHeight() // 建议加上，否则按钮可能拉伸
			[
				SNew(SButton)
				
				.OnClicked_Lambda([TabID]() // 点击时唤起对应的 Tab
				{
				FGlobalTabmanager::Get()->TryInvokeTab(FTabId(TabID));
				return FReply::Handled();
				})
				[
					SNew(STextBlock).Text(NSLOCTEXT("ToolsBox", "OpenBtn", "打开工具"))
				]

			]
		];
}
