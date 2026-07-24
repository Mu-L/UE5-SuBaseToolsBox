// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/Right-ClickOperationTool/AssetAction.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_UOBJECT");
void EmptyLinkFunctionForGeneratedCodeAssetAction() {}

// ********** Begin Cross Module References ********************************************************
BLUTILITY_API UClass* Z_Construct_UClass_UAssetActionUtility(ETypeConstructPhase);
// ********** End Cross Module References **********************************************************

// ********** Begin Same Module References *********************************************************
UPackage* Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAction(ETypeConstructPhase);
TOOLSBOX_API UScriptStruct* Z_Construct_UScriptStruct_FNamingPair(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAction(ETypeConstructPhase);
// ********** End Same Module References ***********************************************************
#define UHT_STRUCT_BASE(INIT) UE::CodeGen::ConstInit::TCompiledInObjectPtr<const FStructBaseChain>(UE::Private::AsStructBaseChain(INIT))

// ********** Begin ScriptStruct FNamingPair *******************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UScriptStruct_FNamingPair_Statics
struct UHT_STATICS
{
	static inline consteval int32 GetStructSize() { return DataSizeOf<FNamingPair>(); }
	static inline consteval int16 GetStructAlignment() { return alignof(FNamingPair); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Prefix_MetaData[] = {
		{ "Category", "Naming" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Suffix_MetaData[] = {
		{ "Category", "Naming" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FNamingPair constinit property declarations ***********************
	static const UECodeGen_Private::FStrPropertyParams NewProp_Prefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Suffix;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FNamingPair constinit property declarations *************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FNamingPair>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct UHT_STATICS

// ********** Begin ScriptStruct FNamingPair Property Definitions **********************************
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Prefix = { "Prefix", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(FNamingPair, Prefix), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Prefix_MetaData), NewProp_Prefix_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Suffix = { "Suffix", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(FNamingPair, Suffix), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Suffix_MetaData), NewProp_Suffix_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Prefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Suffix,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End ScriptStruct FNamingPair Property Definitions ************************************
const UECodeGen_Private::FStructParams UHT_STATICS::StructParams = {
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_ToolsBox,
	nullptr,
	&NewStructOps,
	"NamingPair",
	UHT_STATICS::PropPointers,
	UE_ARRAY_COUNT(UHT_STATICS::PropPointers),
	DataSizeOf<FNamingPair>(),
	alignof(FNamingPair),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)
};
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FNamingPair;
UScriptStruct* Z_Construct_UScriptStruct_FNamingPair(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Outer)
	{
		if (!Z_Registration_Info_UScriptStruct_FNamingPair.OuterSingleton)
		{
			Z_Registration_Info_UScriptStruct_FNamingPair.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FNamingPair, (UObject*)Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase::Outer), TEXT("NamingPair"));
		}
		return Z_Registration_Info_UScriptStruct_FNamingPair.OuterSingleton;
	}
	if (!Z_Registration_Info_UScriptStruct_FNamingPair.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FNamingPair.InnerSingleton, UHT_STATICS::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FNamingPair.InnerSingleton);
}
#undef UHT_STATICS
// ********** End ScriptStruct FNamingPair *********************************************************

// ********** Begin Class UAssetAction Function AddPrefixAndSuffix *********************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix_Statics
struct UHT_STATICS
{
	struct AssetAction_eventAddPrefixAndSuffix_Parms
	{
		FString Prefix;
		FString Suffix;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
		{ "DisplayName", "\xe6\xb7\xbb\xe5\x8a\xa0\xe5\x89\x8d\xe5\x90\x8e\xe7\xbc\x80" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function AddPrefixAndSuffix constinit property declarations ********************
	static const UECodeGen_Private::FStrPropertyParams NewProp_Prefix;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Suffix;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function AddPrefixAndSuffix constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AddPrefixAndSuffix Property Definitions *******************************
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Prefix = { "Prefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAddPrefixAndSuffix_Parms, Prefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Suffix = { "Suffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAddPrefixAndSuffix_Parms, Suffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Prefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Suffix,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function AddPrefixAndSuffix Property Definitions *********************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_UAssetAction, nullptr, "AddPrefixAndSuffix", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::AssetAction_eventAddPrefixAndSuffix_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::AssetAction_eventAddPrefixAndSuffix_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
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
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_UAssetAction_AddTags_Statics
struct UHT_STATICS
{
	struct AssetAction_eventAddTags_Parms
	{
		TArray<FName> TagName;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
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

// ********** Begin Function AddTags constinit property declarations *******************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_TagName_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_TagName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function AddTags constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AddTags Property Definitions ******************************************
const UECodeGen_Private::FNamePropertyParams UHT_STATICS::NewProp_TagName_Inner = { "TagName", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams UHT_STATICS::NewProp_TagName = { "TagName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAddTags_Parms, TagName), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TagName_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TagName,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function AddTags Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_UAssetAction, nullptr, "AddTags", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::AssetAction_eventAddTags_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::AssetAction_eventAddTags_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_AddTags(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
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
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix_Statics
struct UHT_STATICS
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
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
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

// ********** Begin Function AutoAddPrefixAndSuffix constinit property declarations ****************
	static void NewProp_bReplaceExisting_SetBit(void* Obj)
	{
		((AssetAction_eventAutoAddPrefixAndSuffix_Parms*)Obj)->bReplaceExisting = 1;
	}
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
// ********** End Function AutoAddPrefixAndSuffix constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AutoAddPrefixAndSuffix Property Definitions ***************************
const UECodeGen_Private::FBoolPropertyParams UHT_STATICS::NewProp_bReplaceExisting = { "bReplaceExisting", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, nullptr, nullptr, 1, sizeof(bool), sizeof(AssetAction_eventAutoAddPrefixAndSuffix_Parms), &UHT_STATICS::NewProp_bReplaceExisting_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MeshPrefix = { "MeshPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MeshPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MeshSuffix = { "MeshSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MeshSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_BlueprintPrefix = { "BlueprintPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, BlueprintPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_BlueprintSuffix = { "BlueprintSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, BlueprintSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_TexturePrefix = { "TexturePrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TexturePrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_TextureSuffix = { "TextureSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TextureSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MaterialPrefix = { "MaterialPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MaterialSuffix = { "MaterialSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MaterialInstancePrefix = { "MaterialInstancePrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialInstancePrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MaterialInstanceSuffix = { "MaterialInstanceSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialInstanceSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MaterialFunctionPrefix = { "MaterialFunctionPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialFunctionPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_MaterialFunctionSuffix = { "MaterialFunctionSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, MaterialFunctionSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_WorldPrefix = { "WorldPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, WorldPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_WorldSuffix = { "WorldSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, WorldSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_SkeletalPrefix = { "SkeletalPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, SkeletalPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_SkeletalSuffix = { "SkeletalSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, SkeletalSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_TextureRenderPrefix = { "TextureRenderPrefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TextureRenderPrefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_TextureRenderSuffix = { "TextureRenderSuffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventAutoAddPrefixAndSuffix_Parms, TextureRenderSuffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_bReplaceExisting,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MeshPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MeshSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BlueprintPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_BlueprintSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TexturePrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TextureSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaterialPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaterialSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaterialInstancePrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaterialInstanceSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaterialFunctionPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_MaterialFunctionSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_WorldPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_WorldSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SkeletalPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_SkeletalSuffix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TextureRenderPrefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TextureRenderSuffix,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function AutoAddPrefixAndSuffix Property Definitions *****************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_UAssetAction, nullptr, "AutoAddPrefixAndSuffix", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::AssetAction_eventAutoAddPrefixAndSuffix_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::AssetAction_eventAutoAddPrefixAndSuffix_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
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

// ********** Begin Class UAssetAction Function ReplaceText_ ***************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_UAssetAction_ReplaceText__Statics
struct UHT_STATICS
{
	struct AssetAction_eventReplaceText__Parms
	{
		FString OldText;
		FString NewText;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
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

// ********** Begin Function ReplaceText_ constinit property declarations **************************
	static const UECodeGen_Private::FStrPropertyParams NewProp_OldText;
	static const UECodeGen_Private::FStrPropertyParams NewProp_NewText;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ReplaceText_ constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ReplaceText_ Property Definitions *************************************
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_OldText = { "OldText", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventReplaceText__Parms, OldText), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_NewText = { "NewText", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(AssetAction_eventReplaceText__Parms, NewText), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_OldText,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_NewText,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function ReplaceText_ Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_UAssetAction, nullptr, "ReplaceText_", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::AssetAction_eventReplaceText__Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::AssetAction_eventReplaceText__Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UAssetAction_ReplaceText_(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
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

// ********** Begin Class UAssetAction *************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UClass_UAssetAction_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "Tools/Right-ClickOperationTool/AssetAction.h" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/AssetAction.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif // WITH_METADATA

// ********** Begin Class UAssetAction constinit property declarations *****************************
// ********** End Class UAssetAction constinit property declarations *******************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("AddPrefixAndSuffix"), .Pointer = &UAssetAction::execAddPrefixAndSuffix },
		{ .NameUTF8 = UTF8TEXT("AddTags"), .Pointer = &UAssetAction::execAddTags },
		{ .NameUTF8 = UTF8TEXT("AutoAddPrefixAndSuffix"), .Pointer = &UAssetAction::execAutoAddPrefixAndSuffix },
		{ .NameUTF8 = UTF8TEXT("ReplaceText_"), .Pointer = &UAssetAction::execReplaceText_ },
	};
	static FTypeConstructFunc* DependentSingletons[];
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UAssetAction_AddPrefixAndSuffix, "AddPrefixAndSuffix" }, // e09f205d77647cf4ec3c787a1763995bdc83957c
		{ &Z_Construct_UFunction_UAssetAction_AddTags, "AddTags" }, // cb19265b908774d83de5781e944ab5f8c9013dc3
		{ &Z_Construct_UFunction_UAssetAction_AutoAddPrefixAndSuffix, "AutoAddPrefixAndSuffix" }, // bc48b3295378afdd990820bd000013cfca092957
		{ &Z_Construct_UFunction_UAssetAction_ReplaceText_, "ReplaceText_" }, // 8e952eb1ed528f9d7f1fcb518fa8dd71d363eea3
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAssetAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct UHT_STATICS
FTypeConstructFunc* UHT_STATICS::DependentSingletons[] = {
	(FTypeConstructFunc*)Z_Construct_UClass_UAssetActionUtility,
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_ToolsBox,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams UHT_STATICS::ClassParams = {
	&Z_Construct_UClass_UAssetAction,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)
};
static void UAssetAction_StaticRegisterNativesUAssetAction()
{
	UClass* Class = UAssetAction::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, 		MakeConstArrayView(UHT_STATICS::Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UAssetAction;
UClass* Z_Construct_UClass_UAssetAction(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Inner)
	{
		using TClass = UAssetAction;
		if (!Z_Registration_Info_UClass_UAssetAction.InnerSingleton)
		{
			GetPrivateStaticClassBody(
				TClass::StaticPackage(),
				TEXT("AssetAction"),
				Z_Registration_Info_UClass_UAssetAction.InnerSingleton,
				UAssetAction_StaticRegisterNativesUAssetAction,
				DataSizeOf<TClass>(),
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
	if (!Z_Registration_Info_UClass_UAssetAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAssetAction.OuterSingleton, UHT_STATICS::ClassParams);
	}
	return Z_Registration_Info_UClass_UAssetAction.OuterSingleton;
}
#undef UHT_STATICS
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UAssetAction);
UAssetAction::~UAssetAction() {}
// ********** End Class UAssetAction ***************************************************************

// ********** Begin Registration *******************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h__Script_ToolsBox_Statics
struct UHT_STATICS
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ Z_Construct_UScriptStruct_FNamingPair, Z_Construct_UScriptStruct_FNamingPair_Statics::NewStructOps, TEXT("NamingPair"),&Z_Registration_Info_UScriptStruct_FNamingPair, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FNamingPair), 958489036U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UAssetAction, TEXT("UAssetAction"), &Z_Registration_Info_UClass_UAssetAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAssetAction), 3140104579U) },
	};
}; // UHT_STATICS 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h__Script_ToolsBox_c5ca27c9718863cc99fd2c18324fa0ec1f470af4{
	TEXT("/Script/ToolsBox"),
	UHT_STATICS::ClassInfo, UE_ARRAY_COUNT(UHT_STATICS::ClassInfo),
	UHT_STATICS::ScriptStructInfo, UE_ARRAY_COUNT(UHT_STATICS::ScriptStructInfo),
	nullptr, 0,
	nullptr, 0,
};
#undef UHT_STATICS
// ********** End Registration *********************************************************************
#undef UHT_STRUCT_BASE

PRAGMA_ENABLE_DEPRECATION_WARNINGS
