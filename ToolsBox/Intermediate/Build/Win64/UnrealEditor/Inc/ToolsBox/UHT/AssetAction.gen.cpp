// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/Right-ClickOperationTool/AssetAction.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeAssetAction() {}

// ********** Begin Cross Module References ********************************************************
BLUTILITY_API UClass* Z_Construct_UClass_UAssetActionUtility();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FIntPoint();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_ENaniteFallbackTarget();
PHYSICSCORE_API UClass* Z_Construct_UClass_UPhysicalMaterial_NoRegister();
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAction();
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAction_NoRegister();
UPackage* Z_Construct_UPackage__Script_ToolsBox();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UAssetAction Function AddPrefixAndSuffix *********************************
struct Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics
{
	struct AssetAction_eventAddPrefixAndSuffix_Parms
	{
		FString Prefix;
		FString Suffix;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x80\x9a\xe7\x94\xa8" },
		{ "DisplayName", "\xe6\xb7\xbb\xe5\x8a\xa0\xe5\x89\x8d\xe5\x90\x8e\xe7\xbc\x80" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_Prefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Suffix;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::NewProp_Prefix = { "Prefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAddPrefixAndSuffix_Parms, Prefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::NewProp_Suffix = { "Suffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAddPrefixAndSuffix_Parms, Suffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::NewProp_Prefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::NewProp_Suffix,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "AddPrefixAndSuffix", Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::AssetAction_eventAddPrefixAndSuffix_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::AssetAction_eventAddPrefixAndSuffix_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execAddPrefixAndSuffix)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Prefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_Suffix);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddPrefixAndSuffix(Z_Param_Prefix,Z_Param_Suffix);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function AddPrefixAndSuffix ***********************************

// ********** Begin Class UAssetAction Function AddTags ********************************************
struct Z_Construct_UFunction_UAssetAction_AddTags_Statics
{
	struct AssetAction_eventAddTags_Parms
	{
		TArray<FName> TagName;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "Actor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe6\x89\xb9\xe9\x87\x8f\xe6\xb7\xbb\xe5\x8a\xa0\xe6\xa0\x87\xe7\xad\xbe\n" },
#endif
		{ "DisplayName", "\xe6\x89\xb9\xe9\x87\x8f\xe6\xb7\xbb\xe5\x8a\xa0\xe6\xa0\x87\xe7\xad\xbe" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x89\xb9\xe9\x87\x8f\xe6\xb7\xbb\xe5\x8a\xa0\xe6\xa0\x87\xe7\xad\xbe" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_TagName_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TagName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAssetAction_AddTags_Statics::NewProp_TagName_Inner = { "TagName", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UAssetAction_AddTags_Statics::NewProp_TagName = { "TagName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAddTags_Parms, TagName), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_AddTags_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AddTags_Statics::NewProp_TagName_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AddTags_Statics::NewProp_TagName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AddTags_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_AddTags_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "AddTags", Z_Construct_UFunction_UAssetAction_AddTags_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AddTags_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_AddTags_Statics::AssetAction_eventAddTags_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AddTags_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_AddTags_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_AddTags_Statics::AssetAction_eventAddTags_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_AddTags()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_AddTags_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execAddTags)
{
	P_GET_TARRAY(FName,Z_Param_TagName);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddTags(Z_Param_TagName);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function AddTags **********************************************

// ********** Begin Class UAssetAction Function AutoAddPrefixAndSuffix *****************************
struct Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics
{
	struct AssetAction_eventAutoAddPrefixAndSuffix_Parms
	{
		bool bReplaceExisting;
		FString MeshPrefix;
		FString MeshSuffix;
		FString BlueprintPrefix;
		FString BlueprintSuffix;
		FString TexturePrefix;
		FString TextureSuffix;
		FString MaterialPrefix;
		FString MaterialSuffix;
		FString MaterialInstancePrefix;
		FString MaterialInstanceSuffix;
		FString MaterialFunctionPrefix;
		FString MaterialFunctionSuffix;
		FString WorldPrefix;
		FString WorldSuffix;
		FString SkeletalPrefix;
		FString SkeletalSuffix;
		FString TextureRenderPrefix;
		FString TextureRenderSuffix;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x80\x9a\xe7\x94\xa8" },
		{ "CPP_Default_BlueprintPrefix", "BP_" },
		{ "CPP_Default_BlueprintSuffix", "" },
		{ "CPP_Default_bReplaceExisting", "true" },
		{ "CPP_Default_MaterialFunctionPrefix", "MF_" },
		{ "CPP_Default_MaterialFunctionSuffix", "" },
		{ "CPP_Default_MaterialInstancePrefix", "MI_" },
		{ "CPP_Default_MaterialInstanceSuffix", "" },
		{ "CPP_Default_MaterialPrefix", "M_" },
		{ "CPP_Default_MaterialSuffix", "" },
		{ "CPP_Default_MeshPrefix", "SM_" },
		{ "CPP_Default_MeshSuffix", "" },
		{ "CPP_Default_SkeletalPrefix", "SK_" },
		{ "CPP_Default_SkeletalSuffix", "" },
		{ "CPP_Default_TexturePrefix", "T_" },
		{ "CPP_Default_TextureRenderPrefix", "RT_" },
		{ "CPP_Default_TextureRenderSuffix", "" },
		{ "CPP_Default_TextureSuffix", "" },
		{ "CPP_Default_WorldPrefix", "L_" },
		{ "CPP_Default_WorldSuffix", "" },
		{ "DisplayName", "\xe6\xa0\xb9\xe6\x8d\xae\xe8\xb5\x84\xe4\xba\xa7\xe7\xb1\xbb\xe5\x9e\x8b\xe6\xb7\xbb\xe5\x8a\xa0\xe6\x88\x96\xe6\x9b\xbf\xe6\x8d\xa2\xe5\x89\x8d\xe5\x90\x8e\xe7\xbc\x80" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bReplaceExisting_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bReplaceExisting;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MeshPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MeshSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_BlueprintPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_BlueprintSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TexturePrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TextureSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MaterialPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MaterialSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MaterialInstancePrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MaterialInstanceSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MaterialFunctionPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MaterialFunctionSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_WorldPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_WorldSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_SkeletalPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_SkeletalSuffix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TextureRenderPrefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TextureRenderSuffix;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_bReplaceExisting_SetBit(void* Obj)
{
	((AssetAction_eventAutoAddPrefixAndSuffix_Parms*)Obj)->bReplaceExisting = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_bReplaceExisting = { "bReplaceExisting", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventAutoAddPrefixAndSuffix_Parms), &Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_bReplaceExisting_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MeshPrefix = { "MeshPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MeshPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MeshSuffix = { "MeshSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MeshSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_BlueprintPrefix = { "BlueprintPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, BlueprintPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_BlueprintSuffix = { "BlueprintSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, BlueprintSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TexturePrefix = { "TexturePrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TexturePrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TextureSuffix = { "TextureSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TextureSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialPrefix = { "MaterialPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialSuffix = { "MaterialSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialInstancePrefix = { "MaterialInstancePrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialInstancePrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialInstanceSuffix = { "MaterialInstanceSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialInstanceSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialFunctionPrefix = { "MaterialFunctionPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialFunctionPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialFunctionSuffix = { "MaterialFunctionSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialFunctionSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_WorldPrefix = { "WorldPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, WorldPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_WorldSuffix = { "WorldSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, WorldSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_SkeletalPrefix = { "SkeletalPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, SkeletalPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_SkeletalSuffix = { "SkeletalSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, SkeletalSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TextureRenderPrefix = { "TextureRenderPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TextureRenderPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TextureRenderSuffix = { "TextureRenderSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TextureRenderSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_bReplaceExisting,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MeshPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MeshSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_BlueprintPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_BlueprintSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TexturePrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TextureSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialInstancePrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialInstanceSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialFunctionPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_MaterialFunctionSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_WorldPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_WorldSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_SkeletalPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_SkeletalSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TextureRenderPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::NewProp_TextureRenderSuffix,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "AutoAddPrefixAndSuffix", Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::AssetAction_eventAutoAddPrefixAndSuffix_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::AssetAction_eventAutoAddPrefixAndSuffix_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execAutoAddPrefixAndSuffix)
{
	P_GET_UBOOL(Z_Param_bReplaceExisting);
	P_GET_PROPERTY(FStrProperty,Z_Param_MeshPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MeshSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_BlueprintPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_BlueprintSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_TexturePrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_TextureSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MaterialPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MaterialSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MaterialInstancePrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MaterialInstanceSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MaterialFunctionPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_MaterialFunctionSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_WorldPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_WorldSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_SkeletalPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_SkeletalSuffix);
	P_GET_PROPERTY(FStrProperty,Z_Param_TextureRenderPrefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_TextureRenderSuffix);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AutoAddPrefixAndSuffix(Z_Param_bReplaceExisting,Z_Param_MeshPrefix,Z_Param_MeshSuffix,Z_Param_BlueprintPrefix,Z_Param_BlueprintSuffix,Z_Param_TexturePrefix,Z_Param_TextureSuffix,Z_Param_MaterialPrefix,Z_Param_MaterialSuffix,Z_Param_MaterialInstancePrefix,Z_Param_MaterialInstanceSuffix,Z_Param_MaterialFunctionPrefix,Z_Param_MaterialFunctionSuffix,Z_Param_WorldPrefix,Z_Param_WorldSuffix,Z_Param_SkeletalPrefix,Z_Param_SkeletalSuffix,Z_Param_TextureRenderPrefix,Z_Param_TextureRenderSuffix);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function AutoAddPrefixAndSuffix *******************************

// ********** Begin Class UAssetAction Function ClearUnusedNodes ***********************************
struct Z_Construct_UFunction_UAssetAction_ClearUnusedNodes_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe8\x93\x9d\xe5\x9b\xbe" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe6\xb8\x85\xe9\x99\xa4\xe6\x9c\xaa\xe4\xbd\xbf\xe7\x94\xa8\xe8\x8a\x82\xe7\x82\xb9\n" },
#endif
		{ "DisplayName", "\xe6\xb8\x85\xe9\x99\xa4\xe6\x9c\xaa\xe4\xbd\xbf\xe7\x94\xa8\xe8\x8a\x82\xe7\x82\xb9" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\xb8\x85\xe9\x99\xa4\xe6\x9c\xaa\xe4\xbd\xbf\xe7\x94\xa8\xe8\x8a\x82\xe7\x82\xb9" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_ClearUnusedNodes_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "ClearUnusedNodes", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ClearUnusedNodes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_ClearUnusedNodes_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UAssetAction_ClearUnusedNodes()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_ClearUnusedNodes_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execClearUnusedNodes)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ClearUnusedNodes();
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function ClearUnusedNodes *************************************

// ********** Begin Class UAssetAction Function RemoveChar *****************************************
struct Z_Construct_UFunction_UAssetAction_RemoveChar_Statics
{
	struct AssetAction_eventRemoveChar_Parms
	{
		int32 CharCount;
		bool Interval;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x80\x9a\xe7\x94\xa8" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe7\xa7\xbb\xe9\x99\xa4\xe5\x89\x8d(\xe5\x90\x8e)n\xe4\xb8\xaa\xe5\xad\x97\xe7\xac\xa6\n" },
#endif
		{ "CPP_Default_CharCount", "1" },
		{ "CPP_Default_Interval", "false" },
		{ "DisplayName", "\xe7\xa7\xbb\xe9\x99\xa4\xe5\x89\x8d(\xe5\x90\x8e)n\xe4\xb8\xaa\xe5\xad\x97\xe7\xac\xa6\xef\xbc\x88\xe9\xbb\x98\xe8\xae\xa4\xe7\xa7\xbb\xe9\x99\xa4\xe5\x89\x8d""1\xe4\xb8\xaa\xe5\xad\x97\xe7\xac\xa6\xef\xbc\x89" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa7\xbb\xe9\x99\xa4\xe5\x89\x8d(\xe5\x90\x8e)n\xe4\xb8\xaa\xe5\xad\x97\xe7\xac\xa6" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_CharCount;
	static void NewProp_Interval_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_Interval;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::NewProp_CharCount = { "CharCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventRemoveChar_Parms, CharCount), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::NewProp_Interval_SetBit(void* Obj)
{
	((AssetAction_eventRemoveChar_Parms*)Obj)->Interval = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::NewProp_Interval = { "Interval", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventRemoveChar_Parms), &Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::NewProp_Interval_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::NewProp_CharCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::NewProp_Interval,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "RemoveChar", Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::AssetAction_eventRemoveChar_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::AssetAction_eventRemoveChar_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_RemoveChar()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_RemoveChar_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execRemoveChar)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_CharCount);
	P_GET_UBOOL(Z_Param_Interval);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RemoveChar(Z_Param_CharCount,Z_Param_Interval);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function RemoveChar *******************************************

// ********** Begin Class UAssetAction Function RemoveUnusedVariables ******************************
struct Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe8\x93\x9d\xe5\x9b\xbe" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe7\xa7\xbb\xe9\x99\xa4\xe6\x9c\xaa\xe4\xbd\xbf\xe7\x94\xa8\xe5\x8f\x98\xe9\x87\x8f\n" },
#endif
		{ "DisplayName", "\xe7\xa7\xbb\xe9\x99\xa4\xe6\x9c\xaa\xe4\xbd\xbf\xe7\x94\xa8\xe5\x8f\x98\xe9\x87\x8f" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xa7\xbb\xe9\x99\xa4\xe6\x9c\xaa\xe4\xbd\xbf\xe7\x94\xa8\xe5\x8f\x98\xe9\x87\x8f" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "RemoveUnusedVariables", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execRemoveUnusedVariables)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RemoveUnusedVariables();
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function RemoveUnusedVariables ********************************

// ********** Begin Class UAssetAction Function ReplaceText_ ***************************************
struct Z_Construct_UFunction_UAssetAction_ReplaceText__Statics
{
	struct AssetAction_eventReplaceText__Parms
	{
		FString OldText;
		FString NewText;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x80\x9a\xe7\x94\xa8" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe6\x96\x87\xe6\x9c\xac\xe6\x9b\xbf\xe6\x8d\xa2\n" },
#endif
		{ "DisplayName", "\xe6\x96\x87\xe6\x9c\xac\xe6\x9b\xbf\xe6\x8d\xa2" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe6\x96\x87\xe6\x9c\xac\xe6\x9b\xbf\xe6\x8d\xa2" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_OldText;
	static const UECodeGen_Private::FStrPropertyParams NewProp_NewText;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::NewProp_OldText = { "OldText", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventReplaceText__Parms, OldText), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::NewProp_NewText = { "NewText", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventReplaceText__Parms, NewText), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::NewProp_OldText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::NewProp_NewText,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "ReplaceText_", Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::AssetAction_eventReplaceText__Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::AssetAction_eventReplaceText__Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_ReplaceText_()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_ReplaceText__Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execReplaceText_)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_OldText);
	P_GET_PROPERTY(FStrProperty,Z_Param_NewText);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ReplaceText_(Z_Param_OldText,Z_Param_NewText);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function ReplaceText_ *****************************************

// ********** Begin Class UAssetAction Function ResizeTexture **************************************
struct Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics
{
	struct AssetAction_eventResizeTexture_Parms
	{
		FIntPoint NewSize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe7\xba\xb9\xe7\x90\x86" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe9\x87\x8d\xe8\xae\xbe\xe7\xba\xb9\xe7\x90\x86\xe5\xa4\xa7\xe5\xb0\x8f\n" },
#endif
		{ "DisplayName", "\xe9\x87\x8d\xe8\xae\xbe\xe7\xba\xb9\xe7\x90\x86\xe5\xa4\xa7\xe5\xb0\x8f" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe9\x87\x8d\xe8\xae\xbe\xe7\xba\xb9\xe7\x90\x86\xe5\xa4\xa7\xe5\xb0\x8f" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_NewSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::NewProp_NewSize = { "NewSize", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventResizeTexture_Parms, NewSize), Z_Construct_UScriptStruct_FIntPoint, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::NewProp_NewSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "ResizeTexture", Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::AssetAction_eventResizeTexture_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::AssetAction_eventResizeTexture_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_ResizeTexture()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_ResizeTexture_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execResizeTexture)
{
	P_GET_STRUCT(FIntPoint,Z_Param_NewSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ResizeTexture(Z_Param_NewSize);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function ResizeTexture ****************************************

// ********** Begin Class UAssetAction Function SetConvexDecompositionCollision ********************
struct Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics
{
	struct AssetAction_eventSetConvexDecompositionCollision_Parms
	{
		int32 HullCount;
		int32 MaxHullVerts;
		int32 HullPrecision;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x9d\x99\xe6\x80\x81\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe8\xae\xbe\xe7\xbd\xae\xe5\x87\xb8\xe5\x8c\x85\xe5\x88\x86\xe8\xa7\xa3\xe7\xa2\xb0\xe6\x92\x9e\n" },
#endif
		{ "DisplayName", "\xe8\xae\xbe\xe7\xbd\xae\xe5\x87\xb8\xe5\x8c\x85\xe5\x88\x86\xe8\xa7\xa3\xe7\xa2\xb0\xe6\x92\x9e" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xbe\xe7\xbd\xae\xe5\x87\xb8\xe5\x8c\x85\xe5\x88\x86\xe8\xa7\xa3\xe7\xa2\xb0\xe6\x92\x9e" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_HullCount;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxHullVerts;
	static const UECodeGen_Private::FIntPropertyParams NewProp_HullPrecision;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::NewProp_HullCount = { "HullCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSetConvexDecompositionCollision_Parms, HullCount), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::NewProp_MaxHullVerts = { "MaxHullVerts", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSetConvexDecompositionCollision_Parms, MaxHullVerts), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::NewProp_HullPrecision = { "HullPrecision", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSetConvexDecompositionCollision_Parms, HullPrecision), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::NewProp_HullCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::NewProp_MaxHullVerts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::NewProp_HullPrecision,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "SetConvexDecompositionCollision", Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::AssetAction_eventSetConvexDecompositionCollision_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::AssetAction_eventSetConvexDecompositionCollision_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execSetConvexDecompositionCollision)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_HullCount);
	P_GET_PROPERTY(FIntProperty,Z_Param_MaxHullVerts);
	P_GET_PROPERTY(FIntProperty,Z_Param_HullPrecision);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetConvexDecompositionCollision(Z_Param_HullCount,Z_Param_MaxHullVerts,Z_Param_HullPrecision);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function SetConvexDecompositionCollision **********************

// ********** Begin Class UAssetAction Function SetMeshPhysicsMaterial *****************************
struct Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics
{
	struct AssetAction_eventSetMeshPhysicsMaterial_Parms
	{
		UPhysicalMaterial* PhysicsMaterial;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x9d\x99\xe6\x80\x81\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe8\xae\xbe\xe7\xbd\xae\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93\xe7\x89\xa9\xe7\x90\x86\xe6\x9d\x90\xe8\xb4\xa8\n" },
#endif
		{ "DisplayName", "\xe8\xae\xbe\xe7\xbd\xae\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93\xe7\x89\xa9\xe7\x90\x86\xe6\x9d\x90\xe8\xb4\xa8" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe8\xae\xbe\xe7\xbd\xae\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93\xe7\x89\xa9\xe7\x90\x86\xe6\x9d\x90\xe8\xb4\xa8" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PhysicsMaterial;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::NewProp_PhysicsMaterial = { "PhysicsMaterial", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSetMeshPhysicsMaterial_Parms, PhysicsMaterial), Z_Construct_UClass_UPhysicalMaterial_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::NewProp_PhysicsMaterial,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "SetMeshPhysicsMaterial", Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::AssetAction_eventSetMeshPhysicsMaterial_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::AssetAction_eventSetMeshPhysicsMaterial_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execSetMeshPhysicsMaterial)
{
	P_GET_OBJECT(UPhysicalMaterial,Z_Param_PhysicsMaterial);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetMeshPhysicsMaterial(Z_Param_PhysicsMaterial);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function SetMeshPhysicsMaterial *******************************

// ********** Begin Class UAssetAction Function SetNaniteSetting ***********************************
struct Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics
{
	struct AssetAction_eventSetNaniteSetting_Parms
	{
		bool bEnableNanite;
		bool ExplicitTangents;
		bool LerpUVs;
		float KeepPercentTriangles;
		ENaniteFallbackTarget FallbackTarget;
		bool bApplyChanges;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x9d\x99\xe6\x80\x81\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Nanite\xe8\xae\xbe\xe7\xbd\xae\n" },
#endif
		{ "DisplayName", "Nanite\xe8\xae\xbe\xe7\xbd\xae" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Nanite\xe8\xae\xbe\xe7\xbd\xae" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableNanite_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExplicitTangents_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LerpUVs_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_KeepPercentTriangles_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FallbackTarget_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bApplyChanges_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bEnableNanite_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableNanite;
	static void NewProp_ExplicitTangents_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ExplicitTangents;
	static void NewProp_LerpUVs_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_LerpUVs;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_KeepPercentTriangles;
	static const UECodeGen_Private::FBytePropertyParams NewProp_FallbackTarget_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_FallbackTarget;
	static void NewProp_bApplyChanges_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bApplyChanges;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bEnableNanite_SetBit(void* Obj)
{
	((AssetAction_eventSetNaniteSetting_Parms*)Obj)->bEnableNanite = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bEnableNanite = { "bEnableNanite", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventSetNaniteSetting_Parms), &Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bEnableNanite_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableNanite_MetaData), NewProp_bEnableNanite_MetaData) };
void Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_ExplicitTangents_SetBit(void* Obj)
{
	((AssetAction_eventSetNaniteSetting_Parms*)Obj)->ExplicitTangents = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_ExplicitTangents = { "ExplicitTangents", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventSetNaniteSetting_Parms), &Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_ExplicitTangents_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExplicitTangents_MetaData), NewProp_ExplicitTangents_MetaData) };
void Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_LerpUVs_SetBit(void* Obj)
{
	((AssetAction_eventSetNaniteSetting_Parms*)Obj)->LerpUVs = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_LerpUVs = { "LerpUVs", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventSetNaniteSetting_Parms), &Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_LerpUVs_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LerpUVs_MetaData), NewProp_LerpUVs_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_KeepPercentTriangles = { "KeepPercentTriangles", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSetNaniteSetting_Parms, KeepPercentTriangles), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_KeepPercentTriangles_MetaData), NewProp_KeepPercentTriangles_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_FallbackTarget_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_FallbackTarget = { "FallbackTarget", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSetNaniteSetting_Parms, FallbackTarget), Z_Construct_UEnum_Engine_ENaniteFallbackTarget, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FallbackTarget_MetaData), NewProp_FallbackTarget_MetaData) }; // 3054841591
void Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bApplyChanges_SetBit(void* Obj)
{
	((AssetAction_eventSetNaniteSetting_Parms*)Obj)->bApplyChanges = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bApplyChanges = { "bApplyChanges", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventSetNaniteSetting_Parms), &Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bApplyChanges_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bApplyChanges_MetaData), NewProp_bApplyChanges_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bEnableNanite,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_ExplicitTangents,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_LerpUVs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_KeepPercentTriangles,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_FallbackTarget_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_FallbackTarget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::NewProp_bApplyChanges,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "SetNaniteSetting", Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::AssetAction_eventSetNaniteSetting_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::AssetAction_eventSetNaniteSetting_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_SetNaniteSetting()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_SetNaniteSetting_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execSetNaniteSetting)
{
	P_GET_UBOOL_REF(Z_Param_Out_bEnableNanite);
	P_GET_UBOOL_REF(Z_Param_Out_ExplicitTangents);
	P_GET_UBOOL_REF(Z_Param_Out_LerpUVs);
	P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_KeepPercentTriangles);
	P_GET_ENUM_REF(ENaniteFallbackTarget,Z_Param_Out_FallbackTarget);
	P_GET_UBOOL_REF(Z_Param_Out_bApplyChanges);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetNaniteSetting(Z_Param_Out_bEnableNanite,Z_Param_Out_ExplicitTangents,Z_Param_Out_LerpUVs,Z_Param_Out_KeepPercentTriangles,(ENaniteFallbackTarget&)(Z_Param_Out_FallbackTarget),Z_Param_Out_bApplyChanges);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function SetNaniteSetting *************************************

// ********** Begin Class UAssetAction Function SimplifyMesh ***************************************
struct Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics
{
	struct AssetAction_eventSimplifyMesh_Parms
	{
		float Percent;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "\xe9\x9d\x99\xe6\x80\x81\xe7\xbd\x91\xe6\xa0\xbc\xe4\xbd\x93" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\xe7\xae\x80\xe5\x8c\x96\xe6\xa8\xa1\xe5\x9e\x8b\n" },
#endif
		{ "DisplayName", "\xe7\xae\x80\xe5\x8c\x96\xe6\xa8\xa1\xe5\x9e\x8b" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe7\xae\x80\xe5\x8c\x96\xe6\xa8\xa1\xe5\x9e\x8b" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Percent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::NewProp_Percent = { "Percent", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventSimplifyMesh_Parms, Percent), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::NewProp_Percent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UAssetAction, nullptr, "SimplifyMesh", Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::AssetAction_eventSimplifyMesh_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::AssetAction_eventSimplifyMesh_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_SimplifyMesh()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAssetAction_SimplifyMesh_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UAssetAction::execSimplifyMesh)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Percent);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SimplifyMesh(Z_Param_Percent);
	P_NATIVE_END;
}
// ********** End Class UAssetAction Function SimplifyMesh *****************************************

// ********** Begin Class UAssetAction *************************************************************
void UAssetAction::StaticRegisterNativesUAssetAction()
{
	UClass* Class = UAssetAction::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AddPrefixAndSuffix", &UAssetAction::execAddPrefixAndSuffix },
		{ "AddTags", &UAssetAction::execAddTags },
		{ "AutoAddPrefixAndSuffix", &UAssetAction::execAutoAddPrefixAndSuffix },
		{ "ClearUnusedNodes", &UAssetAction::execClearUnusedNodes },
		{ "RemoveChar", &UAssetAction::execRemoveChar },
		{ "RemoveUnusedVariables", &UAssetAction::execRemoveUnusedVariables },
		{ "ReplaceText_", &UAssetAction::execReplaceText_ },
		{ "ResizeTexture", &UAssetAction::execResizeTexture },
		{ "SetConvexDecompositionCollision", &UAssetAction::execSetConvexDecompositionCollision },
		{ "SetMeshPhysicsMaterial", &UAssetAction::execSetMeshPhysicsMaterial },
		{ "SetNaniteSetting", &UAssetAction::execSetNaniteSetting },
		{ "SimplifyMesh", &UAssetAction::execSimplifyMesh },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UAssetAction;
UClass* UAssetAction::GetPrivateStaticClass()
{
	using TClass = UAssetAction;
	if (!Z_Registration_Info_UClass_UAssetAction.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("AssetAction"),
			Z_Registration_Info_UClass_UAssetAction.InnerSingleton,
			StaticRegisterNativesUAssetAction,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UAssetAction.InnerSingleton;
}
UClass* Z_Construct_UClass_UAssetAction_NoRegister()
{
	return UAssetAction::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UAssetAction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Object" },
		{ "IncludePath", "Tools/Right-ClickOperationTool/AssetAction.h" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix, "AddPrefixAndSuffix" }, // 3666730708
		{ &Z_Construct_UFunction_UAssetAction_AddTags, "AddTags" }, // 1542200048
		{ &Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix, "AutoAddPrefixAndSuffix" }, // 2000953706
		{ &Z_Construct_UFunction_UAssetAction_ClearUnusedNodes, "ClearUnusedNodes" }, // 1499269984
		{ &Z_Construct_UFunction_UAssetAction_RemoveChar, "RemoveChar" }, // 1928973379
		{ &Z_Construct_UFunction_UAssetAction_RemoveUnusedVariables, "RemoveUnusedVariables" }, // 1683788304
		{ &Z_Construct_UFunction_UAssetAction_ReplaceText_, "ReplaceText_" }, // 1249944200
		{ &Z_Construct_UFunction_UAssetAction_ResizeTexture, "ResizeTexture" }, // 2845777936
		{ &Z_Construct_UFunction_UAssetAction_SetConvexDecompositionCollision, "SetConvexDecompositionCollision" }, // 2051899091
		{ &Z_Construct_UFunction_UAssetAction_SetMeshPhysicsMaterial, "SetMeshPhysicsMaterial" }, // 4201751148
		{ &Z_Construct_UFunction_UAssetAction_SetNaniteSetting, "SetNaniteSetting" }, // 731724126
		{ &Z_Construct_UFunction_UAssetAction_SimplifyMesh, "SimplifyMesh" }, // 1836435205
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAssetAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UAssetAction_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UAssetActionUtility,
	(UObject* (*)())Z_Construct_UPackage__Script_ToolsBox,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UAssetAction_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UAssetAction_Statics::ClassParams = {
	&UAssetAction::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UAssetAction_Statics::Class_MetaDataParams), Z_Construct_UClass_UAssetAction_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UAssetAction()
{
	if (!Z_Registration_Info_UClass_UAssetAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAssetAction.OuterSingleton, Z_Construct_UClass_UAssetAction_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UAssetAction.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UAssetAction);
UAssetAction::~UAssetAction() {}
// ********** End Class UAssetAction ***************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h__Script_ToolsBox_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UAssetAction, UAssetAction::StaticClass, TEXT("UAssetAction"), &Z_Registration_Info_UClass_UAssetAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAssetAction), 2418366138U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h__Script_ToolsBox_142672401(TEXT("/Script/ToolsBox"),
	Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h__Script_ToolsBox_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h__Script_ToolsBox_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
