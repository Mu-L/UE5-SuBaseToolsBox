// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "Materials/MaterialInterface.h"
#include "Widgets/SCompoundWidget.h"

struct FParamMappingPair
{
	FString TargetParamName;
	FString SourceParamName;
};
 
class SMaterialTttributeTransfer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialTttributeTransfer) {}
	SLATE_END_ARGS()
 
	void Construct(const FArguments& InArgs);
 
private:
	// UI 容器与数据
	TSharedPtr<SVerticalBox> MappingContainer;
	TArray<TSharedPtr<FParamMappingPair>> MappingList;
	TWeakObjectPtr<UMaterialInterface> TargetMasterMaterial;
 
	// 持久化变量
	FString SaveConfigFileName;
	FString TargetSavePath; // 新增：保存路径变量
 
	// 日志相关
	TSharedPtr<class SMultiLineEditableText> LogWindow;
	FText LogContent;
	void AppendLog(const FString& InLog);
 
	// --- 内部辅助函数 ---
	void AddMappingRow();
	TSharedRef<SWidget> CreateMappingRowWidget(TSharedPtr<FParamMappingPair> InPair);
	void RefreshMappingUI();
    
	FString GetSaveDirectory() const;
	FString GetFullConfigPath() const;
 
	void SaveSettings();
	void LoadSettings();
 
	FReply OnExecuteTransfer();
	void UpdateCurrentPathFromContentBrowser();
 
	void OnMasterMaterialChanged(const FAssetData& AssetData);
	FString GetMasterMaterialPath() const;
};