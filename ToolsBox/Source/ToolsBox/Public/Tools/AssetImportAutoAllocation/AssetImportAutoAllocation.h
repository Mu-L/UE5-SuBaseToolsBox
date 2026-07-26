// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"
//复制本示例模版,并改成自己的感觉名字后,最后到Tools.cpp内对应结构体变量登记即可
/** 映射配置结构体 */
struct FAssetTypeMapping
{
	// 使用弱引用防止 GC 问题
	TWeakObjectPtr<UClass> AssetClass;
	FString TargetPath;
 
	FAssetTypeMapping() : AssetClass(nullptr), TargetPath(TEXT("/Game/Organized/")) {}
};
 
class SAssetImportAutoAllocation : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetImportAutoAllocation) {}
	SLATE_END_ARGS()
 
	void Construct(const FArguments& InArgs);
 
private:
	/** 刷新列表 UI */
	void RefreshMappingList();
 
	/** 创建单行映射的小部件 */
	TSharedRef<SWidget> CreateRowWidget(TSharedPtr<FAssetTypeMapping> Item);
 
	/** 核心数据源 */
	TArray<TSharedPtr<FAssetTypeMapping>> MappingItems;
 
	/** 存放行内容的容器 */
	TSharedPtr<SVerticalBox> ListContainer;
};