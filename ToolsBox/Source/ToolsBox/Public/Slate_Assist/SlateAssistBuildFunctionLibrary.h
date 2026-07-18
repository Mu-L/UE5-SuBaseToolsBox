#pragma once
#include "Templates/SharedPointer.h"

class SWidget;

class SlateAssistBuildFunctionLibrary
{
public:

	//这个函数用来循环往工具箱滚动框添加预设工具块
static TSharedRef<SWidget> MakeToolBlock(const FText& ToolName, const FText& Description, const FName& IconName,const FName& TabID);
	
};
