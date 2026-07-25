// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "Materials/MaterialInterface.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"
// 映射参数的结构体
struct FParamMappingPair
{
	FString TargetParamName; // 母材质里的参数名
	FString SourceParamName; // 原始材质里的变量名
};
 
class SMaterialTttributeTransfer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialTttributeTransfer) {}
	SLATE_END_ARGS()
 
	void Construct(const FArguments& InArgs);
 
private:
	// UI 组件引用
	TSharedPtr<SVerticalBox> MappingContainer;
	TArray<TSharedPtr<FParamMappingPair>> MappingList;
 
	// 选择的目标母材质
	TWeakObjectPtr<UMaterialInterface> TargetMasterMaterial;
 
	// 生成单行输入 UI
	void AddMappingRow();
	TSharedRef<SWidget> CreateMappingRowWidget(TSharedPtr<FParamMappingPair> InPair);
 
	// 核心逻辑
	FReply OnExecuteTransfer();
	FString GetCurrentContentBrowserPath();
    
	// 属性器回调
	void OnMasterMaterialChanged(const FAssetData& AssetData);
	FString GetMasterMaterialPath() const;
};
