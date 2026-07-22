// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/AssetActionUtility/ScriptedAction.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_UOBJECT");
void EmptyLinkFunctionForGeneratedCodeScriptedAction() {}

// ********** Begin Cross Module References ********************************************************
BLUTILITY_API UClass* Z_Construct_UClass_UAssetActionUtility(ETypeConstructPhase);
// ********** End Cross Module References **********************************************************

// ********** Begin Same Module References *********************************************************
UPackage* Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UScriptedAction(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UScriptedAction(ETypeConstructPhase);
// ********** End Same Module References ***********************************************************
#define UHT_STRUCT_BASE(INIT) UE::CodeGen::ConstInit::TCompiledInObjectPtr<const FStructBaseChain>(UE::Private::AsStructBaseChain(INIT))

// ********** Begin Class UScriptedAction Function AddPrefixAndSuffix ******************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_UScriptedAction_AddPrefixAndSuffix_Statics
struct UHT_STATICS
{
	struct ScriptedAction_eventAddPrefixAndSuffix_Parms
	{
		FString Prefix;
		FString Suffix;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
		{ "DisplayName", "\xe6\xb7\xbb\xe5\x8a\xa0\xe5\x89\x8d\xe5\x90\x8e\xe7\xbc\x80" },
		{ "ModuleRelativePath", "Public/Tools/AssetActionUtility/ScriptedAction.h" },
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
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Prefix = { "Prefix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ScriptedAction_eventAddPrefixAndSuffix_Parms, Prefix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_Suffix = { "Suffix", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(ScriptedAction_eventAddPrefixAndSuffix_Parms, Suffix), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Prefix,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_Suffix,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function AddPrefixAndSuffix Property Definitions *********************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_UScriptedAction, nullptr, "AddPrefixAndSuffix", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::ScriptedAction_eventAddPrefixAndSuffix_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::ScriptedAction_eventAddPrefixAndSuffix_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UScriptedAction_AddPrefixAndSuffix(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(UScriptedAction::execAddPrefixAndSuffix)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Prefix);
	P_GET_PROPERTY(FStrProperty,Z_Param_Suffix);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddPrefixAndSuffix(Z_Param_Prefix,Z_Param_Suffix);
	P_NATIVE_END;
}
// ********** End Class UScriptedAction Function AddPrefixAndSuffix ********************************

// ********** Begin Class UScriptedAction **********************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UClass_UScriptedAction_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Object" },
		{ "IncludePath", "Tools/AssetActionUtility/ScriptedAction.h" },
		{ "ModuleRelativePath", "Public/Tools/AssetActionUtility/ScriptedAction.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif // WITH_METADATA

// ********** Begin Class UScriptedAction constinit property declarations **************************
// ********** End Class UScriptedAction constinit property declarations ****************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("AddPrefixAndSuffix"), .Pointer = &UScriptedAction::execAddPrefixAndSuffix },
	};
	static FTypeConstructFunc* DependentSingletons[];
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UScriptedAction_AddPrefixAndSuffix, "AddPrefixAndSuffix" }, // bd3134135697b9028e7513cb895a8a2abce0f263
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UScriptedAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct UHT_STATICS
FTypeConstructFunc* UHT_STATICS::DependentSingletons[] = {
	(FTypeConstructFunc*)Z_Construct_UClass_UAssetActionUtility,
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_ToolsBox,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams UHT_STATICS::ClassParams = {
	&Z_Construct_UClass_UScriptedAction,
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
static void UScriptedAction_StaticRegisterNativesUScriptedAction()
{
	UClass* Class = UScriptedAction::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, 		MakeConstArrayView(UHT_STATICS::Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UScriptedAction;
UClass* Z_Construct_UClass_UScriptedAction(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Inner)
	{
		using TClass = UScriptedAction;
		if (!Z_Registration_Info_UClass_UScriptedAction.InnerSingleton)
		{
			GetPrivateStaticClassBody(
				TClass::StaticPackage(),
				TEXT("ScriptedAction"),
				Z_Registration_Info_UClass_UScriptedAction.InnerSingleton,
				UScriptedAction_StaticRegisterNativesUScriptedAction,
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
		return Z_Registration_Info_UClass_UScriptedAction.InnerSingleton;
	}
	if (!Z_Registration_Info_UClass_UScriptedAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UScriptedAction.OuterSingleton, UHT_STATICS::ClassParams);
	}
	return Z_Registration_Info_UClass_UScriptedAction.OuterSingleton;
}
#undef UHT_STATICS
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UScriptedAction);
UScriptedAction::~UScriptedAction() {}
// ********** End Class UScriptedAction ************************************************************

// ********** Begin Registration *******************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetActionUtility_ScriptedAction_h__Script_ToolsBox_Statics
struct UHT_STATICS
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UScriptedAction, TEXT("UScriptedAction"), &Z_Registration_Info_UClass_UScriptedAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UScriptedAction), 3225887795U) },
	};
}; // UHT_STATICS 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetActionUtility_ScriptedAction_h__Script_ToolsBox_6386797187c9ab900a0331f37255e093241e4654{
	TEXT("/Script/ToolsBox"),
	UHT_STATICS::ClassInfo, UE_ARRAY_COUNT(UHT_STATICS::ClassInfo),
	nullptr, 0,
	nullptr, 0,
	nullptr, 0,
};
#undef UHT_STATICS
// ********** End Registration *********************************************************************
#undef UHT_STRUCT_BASE

PRAGMA_ENABLE_DEPRECATION_WARNINGS
