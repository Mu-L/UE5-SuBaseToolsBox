#pragma once

#include "CoreMinimal.h"
#include "Slate_Assist/FIconStyle.h" // 引入化妆间，为了拿样式名字
#include "Framework/Commands/Commands.h" // 引入命令基类


class FOpenToolsBox_Command : public TCommands<FOpenToolsBox_Command>
{
public:
	FOpenToolsBox_Command()
	: TCommands<FOpenToolsBox_Command>
	(
		TEXT("ToolsBox"), // 按钮的“姓”，很重要，要跟图标匹配
		NSLOCTEXT("Contexts", "SuBaseToolsBox_PluginCommand", "SuBaseToolsBox_PluginCommand"), // 在设置里显示的组名
		NAME_None,
		FIconStyle::Get_IconsName()
	) {}

	// 必须重写的函数：真正登记按钮的地方
	virtual void RegisterCommands() override;

	// 定义一个“按钮指针”，以后代码里就靠这个变量来找这个按钮
	TSharedPtr<FUICommandInfo> ToolsBox_CommandInfo;
};