// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialIRCommon.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

/**
 * 
 */
struct FChannelMapping
{
	FString KeyName;
	int32 FoundIndex;
	EMaterialProperty TargetProp;
 
	// 必须重载小于号，以便 FoundChannels.Sort() 正常工作
	bool operator<(const FChannelMapping& Other) const
	{
		return FoundIndex < Other.FoundIndex;
	}
};

struct FChannelKeyword
{
	FString Key; // "Roughness", "Metallic", "AmbientOcclusion"
	int32 FoundIndex;
	EMaterialProperty TargetProp;
 
	bool operator<(const FChannelKeyword& Other) const { return FoundIndex < Other.FoundIndex; }
};

struct FImportFolderTask
{
	FString FolderName;
	FString MeshPath; // FBX 路径
	// Key 是贴图类型（如 "BaseColor"），Value 是贴图的绝对文件路径
	TMap<FString, FString> TextureMap; 
};


class  SImport_MM : public  SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImport_MM) {}
	SLATE_END_ARGS()
	SImport_MM();
	
	void Construct(const FArguments& InArgs);


	FString SelectedFolderPath;
 
	/** 路径显示框的引用 */
	TSharedPtr<class SEditableTextBox> PathTextBox;
 
	/** 点击“浏览”按钮：弹出系统文件夹选择框 */
	FReply OnBrowseClicked();
 
	/** 点击“开始导入”按钮：执行扫描和导入流程 */
	FReply OnStartImportClicked();
 
	/** 核心逻辑：将单个任务导入引擎并自动化设置材质 */
	void ExecuteImportTask(const FImportFolderTask& Task);
	



};
