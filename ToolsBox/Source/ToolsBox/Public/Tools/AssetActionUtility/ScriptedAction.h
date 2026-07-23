// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <AclAPI.h>

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/AssetActionUtility.h"
#include "ScriptedAction.generated.h"

/**
 * 
 */
UCLASS()
class TOOLSBOX_API UScriptedAction : public UAssetActionUtility
{
	GENERATED_BODY()


public:


	//通过初始化参数列表让构造函数执行父类构造函数
	UScriptedAction(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable,DisplayName="添加前后缀",CallInEditor)
	void  AddPrefixAndSuffix(FString Prefix, FString Suffix);

	//文本替换
	UFUNCTION(BlueprintCallable,DisplayName="文本替换",CallInEditor)
	void  ReplaceText_(FString OldText, FString NewText);
	
	
};
