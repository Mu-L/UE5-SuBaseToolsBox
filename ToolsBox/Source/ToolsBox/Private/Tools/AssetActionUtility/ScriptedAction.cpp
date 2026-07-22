// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/AssetActionUtility/ScriptedAction.h"

#include "EditorUtilityLibrary.h" // 位于 Blutility 模块


UScriptedAction::UScriptedAction(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// 你的初始化逻辑
	SupportedClasses.Add(UObject::StaticClass());
}

void UScriptedAction::AddPrefixAndSuffix(FString Prefix, FString Suffix)
{
	// 1. 获取选中的资产
	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
 
	for (UObject* Asset : SelectedAssets)
	{
		if (!Asset) continue;
 
		FString OldName = Asset->GetName();
		FString NewName = Prefix + OldName + Suffix;
        
		// 获取包路径 (例如 /Game/Textures/)
		FString PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
		FString NewObjectPath = PackagePath + TEXT("/") + NewName;
 
		// 2. 执行改名（自动处理重定向器）
		UEditorUtilityLibrary::RenameAsset(Asset, NewObjectPath);
	}
}
