// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AssetAutoSaveSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAssetImportRule
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere, Category = "Rule")
	TSoftClassPtr<UObject> AssetClass;
 
	UPROPERTY(EditAnywhere, Category = "Rule")
	FString TargetPath;
};
 
UCLASS(Config=Editor, DefaultConfig, meta=(DisplayName="资产自动分类设置"))
class TOOLSBOX_API UAssetAutoSaveSettings : public UDeveloperSettings
{
	GENERATED_BODY()
 
public:
	UAssetAutoSaveSettings() {}
 
	// 存储所有映射规则
	UPROPERTY(Config, EditAnywhere, Category = "Classification")
	TArray<FAssetImportRule> ImportRules;
 
	static UAssetAutoSaveSettings* Get() { return GetMutableDefault<UAssetAutoSaveSettings>(); }
};
