// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/AssetActionUtility/ScriptedAction.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h" // 位于 Blutility 模块
#include "Slate_Assist/SlateAssistBuildFunctionLibrary.h"


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
 
		// 1. 获取原始信息
		FString OldName = Asset->GetName();
		FString NewName = Prefix + OldName + Suffix;
        
		// 如果名字没变，直接跳过
		if (OldName.Equals(NewName)) continue;
 
		// 2. 获取路径
		// GetPathName() 返回类似 /Game/Items/MyAsset.MyAsset
		FString FullPath = Asset->GetPathName();
		// 获取包路径，如 /Game/Items/
		FString PackagePath = FPackageName::GetLongPackagePath(FullPath);
		// 构建完整的新路径 /Game/Items/NewName
		FString NewObjectPath = FPaths::Combine(PackagePath, NewName);
 
		// 3. 使用 EditorAssetLibrary 进行重命名
		// 该方法会自动处理底层的包创建、重定向器逻辑
		if (UEditorAssetLibrary::RenameAsset(FullPath, NewObjectPath))
		{
			SlateAssistBuildFunctionLibrary::SpawnNotifiy(INVTEXT("成功重命名"),0.3f,1.0f,SNotificationItem::ECompletionState::CS_Success);
		}
		else
		{
			SlateAssistBuildFunctionLibrary::SpawnNotifiy(INVTEXT("重命名失败 请检查目标路径是否已存在同名资产或重定向器"),0.3f,1.0f,SNotificationItem::ECompletionState::CS_Fail);
		}
	}
}

void UScriptedAction::ReplaceText_(FString OldText, FString NewText)
{
	if (OldText.IsEmpty())
	{
		SlateAssistBuildFunctionLibrary::SpawnNotifiy(INVTEXT("请输入要查找的文本"),0.3f,1.0f,SNotificationItem::ECompletionState::CS_Fail);
		return;
	}

	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
	
}
