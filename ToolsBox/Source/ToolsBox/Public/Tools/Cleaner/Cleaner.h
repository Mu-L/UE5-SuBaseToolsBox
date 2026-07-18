// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"


class  SCleaner : public SCompoundWidget
{
public:
	
	
	SLATE_BEGIN_ARGS(SCleaner) {}
	SLATE_END_ARGS()
	SCleaner();
	
	void Construct(const FArguments& InArgs)
	{
		ChildSlot
		[
			SNew(STextBlock).Text(NSLOCTEXT("SCleaner", "这是清理工具的独立界面", "这是清理工具的独立界面"))
			// 在这里编写该工具复杂的 UI 排布
		];
	}
	

};
