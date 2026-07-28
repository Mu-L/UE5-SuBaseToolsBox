// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "Materials/MaterialInterface.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/SMultiLineEditableText.h"

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
	TSharedPtr<SVerticalBox> MappingContainer;
	TArray<TSharedPtr<FParamMappingPair>> MappingList;
	TWeakObjectPtr<UMaterialInterface> TargetMasterMaterial;
 
	FString SaveConfigFileName;
	FString TargetSavePath;

	// 勾选后：生成的新材质实例保存在源材质各自所在的文件夹中；默认勾选
	bool bSaveToRespectiveFolders = true;
 
	// 日志组件修复：添加 ScrollBox 引用
	TSharedPtr<SScrollBox> LogScrollBox;
	TSharedPtr<SMultiLineEditableText> LogWindow;
	FText LogContent;
	void AppendLog(const FString& InLog);
 
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