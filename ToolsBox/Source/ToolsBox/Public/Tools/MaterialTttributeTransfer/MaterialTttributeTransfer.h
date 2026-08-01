// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
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
	uint8 bSaveToRespectiveFolders = true;

	// 输出类型控制：两个不能同时打钩，默认都不勾选（按源材质类型生成）
	uint8 bForceGenerateMaterial = false;  // 统一生成材质类
	uint8 bForceGenerateInstance = false; // 统一生成材质实例
 
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

	// 追溯母材质链，找到最底层的基础材质类（UMaterial），用于"生成材质类"时复制其材质图
	UMaterial* FindBaseMaterialTemplate(UMaterialInterface* InMat) const;
	// 在同一包路径下生成不重名的资产名，避免命名冲突导致创建失败
	void MakeUniqueAssetName(const FString& PackagePath, const FString& BaseName, FString& OutName) const;
	// 将源材质（按映射表）的参数值写入目标材质（实例或材质类均可）
	void ApplyParameterValues(UMaterialInterface* Target, UMaterialInterface* Source) const;
 
	void SaveSettings();
	void LoadSettings();
 
	FReply OnExecuteTransfer();
	void UpdateCurrentPathFromContentBrowser(); 
 
	void OnMasterMaterialChanged(const FAssetData& AssetData);
	FString GetMasterMaterialPath() const;
};