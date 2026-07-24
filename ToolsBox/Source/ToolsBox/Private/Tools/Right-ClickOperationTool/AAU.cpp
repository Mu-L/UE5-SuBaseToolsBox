// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Right-ClickOperationTool/AAU.h"

void SAAU::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(STextBlock).Text(NSLOCTEXT("SBlankTemplateTool", "工具空白模版", "工具空白模版"))
			// 在这里编写该工具 UI 
		];
}
