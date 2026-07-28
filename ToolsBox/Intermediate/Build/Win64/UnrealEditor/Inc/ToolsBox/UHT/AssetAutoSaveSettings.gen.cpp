// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_UOBJECT");
void EmptyLinkFunctionForGeneratedCodeAssetAutoSaveSettings() {}

// ********** Begin Cross Module References ********************************************************
DEVELOPERSETTINGS_API UClass* Z_Construct_UClass_UDeveloperSettings(ETypeConstructPhase);
COREUOBJECT_API UClass* Z_Construct_UClass_UClass(ETypeConstructPhase);
COREUOBJECT_API UClass* Z_Construct_UClass_UObject(ETypeConstructPhase);
// ********** End Cross Module References **********************************************************

// ********** Begin Same Module References *********************************************************
UPackage* Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAutoSaveSettings(ETypeConstructPhase);
TOOLSBOX_API UScriptStruct* Z_Construct_UScriptStruct_FAssetImportRule(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAutoSaveSettings(ETypeConstructPhase);
// ********** End Same Module References ***********************************************************
#define UHT_STRUCT_BASE(INIT) UE::CodeGen::ConstInit::TCompiledInObjectPtr<const FStructBaseChain>(UE::Private::AsStructBaseChain(INIT))

// ********** Begin ScriptStruct FAssetImportRule **************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UScriptStruct_FAssetImportRule_Statics
struct UHT_STATICS
{
	static inline consteval int32 GetStructSize() { return DataSizeOf<FAssetImportRule>(); }
	static inline consteval int16 GetStructAlignment() { return alignof(FAssetImportRule); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "ModuleRelativePath", "Public/Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AssetClass_MetaData[] = {
		{ "Category", "Rule" },
		{ "ModuleRelativePath", "Public/Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TargetPath_MetaData[] = {
		{ "Category", "Rule" },
		{ "ModuleRelativePath", "Public/Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FAssetImportRule constinit property declarations ******************
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_AssetClass;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TargetPath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FAssetImportRule constinit property declarations ********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAssetImportRule>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct UHT_STATICS

// ********** Begin ScriptStruct FAssetImportRule Property Definitions *****************************
const UECodeGen_Private::FSoftClassPropertyParams UHT_STATICS::NewProp_AssetClass = { "AssetClass", nullptr, (EPropertyFlags)0x0014000000000001, UECodeGen_Private::EPropertyGenFlags::SoftClass, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetImportRule, AssetClass), Z_Construct_UClass_UObject, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AssetClass_MetaData), NewProp_AssetClass_MetaData) };
const UECodeGen_Private::FStrPropertyParams UHT_STATICS::NewProp_TargetPath = { "TargetPath", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, nullptr, nullptr, 1, STRUCT_OFFSET(FAssetImportRule, TargetPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TargetPath_MetaData), NewProp_TargetPath_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_AssetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_TargetPath,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End ScriptStruct FAssetImportRule Property Definitions *******************************
const UECodeGen_Private::FStructParams UHT_STATICS::StructParams = {
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_ToolsBox,
	nullptr,
	&NewStructOps,
	"AssetImportRule",
	UHT_STATICS::PropPointers,
	UE_ARRAY_COUNT(UHT_STATICS::PropPointers),
	DataSizeOf<FAssetImportRule>(),
	alignof(FAssetImportRule),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)
};
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FAssetImportRule;
UScriptStruct* Z_Construct_UScriptStruct_FAssetImportRule(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Outer)
	{
		if (!Z_Registration_Info_UScriptStruct_FAssetImportRule.OuterSingleton)
		{
			Z_Registration_Info_UScriptStruct_FAssetImportRule.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FAssetImportRule, (UObject*)Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase::Outer), TEXT("AssetImportRule"));
		}
		return Z_Registration_Info_UScriptStruct_FAssetImportRule.OuterSingleton;
	}
	if (!Z_Registration_Info_UScriptStruct_FAssetImportRule.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FAssetImportRule.InnerSingleton, UHT_STATICS::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FAssetImportRule.InnerSingleton);
}
#undef UHT_STATICS
// ********** End ScriptStruct FAssetImportRule ****************************************************

// ********** Begin Class UAssetAutoSaveSettings ***************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UClass_UAssetAutoSaveSettings_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "DisplayName", "\xe8\xb5\x84\xe4\xba\xa7\xe8\x87\xaa\xe5\x8a\xa8\xe5\x88\x86\xe7\xb1\xbb\xe8\xae\xbe\xe7\xbd\xae" },
		{ "IncludePath", "Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h" },
		{ "ModuleRelativePath", "Public/Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ImportRules_MetaData[] = {
		{ "Category", "Classification" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xe5\xad\x98\xe5\x82\xa8\xe6\x89\x80\xe6\x9c\x89\xe6\x98\xa0\xe5\xb0\x84\xe8\xa7\x84\xe5\x88\x99\n" },
#endif
		{ "ModuleRelativePath", "Public/Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xad\x98\xe5\x82\xa8\xe6\x89\x80\xe6\x9c\x89\xe6\x98\xa0\xe5\xb0\x84\xe8\xa7\x84\xe5\x88\x99" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UAssetAutoSaveSettings constinit property declarations *******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ImportRules_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ImportRules;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UAssetAutoSaveSettings constinit property declarations *********************
	static FTypeConstructFunc* DependentSingletons[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAssetAutoSaveSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct UHT_STATICS

// ********** Begin Class UAssetAutoSaveSettings Property Definitions ******************************
const UECodeGen_Private::FStructPropertyParams UHT_STATICS::NewProp_ImportRules_Inner = { "ImportRules", nullptr, (EPropertyFlags)0x0000000000004000, UECodeGen_Private::EPropertyGenFlags::Struct, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FAssetImportRule, METADATA_PARAMS(0, nullptr) }; // 29c08ee4b9c5725a9c96f3d8521914c29094432f
const UECodeGen_Private::FArrayPropertyParams UHT_STATICS::NewProp_ImportRules = { "ImportRules", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Array, nullptr, nullptr, 1, STRUCT_OFFSET(UAssetAutoSaveSettings, ImportRules), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ImportRules_MetaData), NewProp_ImportRules_MetaData) }; // 29c08ee4b9c5725a9c96f3d8521914c29094432f
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ImportRules_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_ImportRules,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Class UAssetAutoSaveSettings Property Definitions ********************************
FTypeConstructFunc* UHT_STATICS::DependentSingletons[] = {
	(FTypeConstructFunc*)Z_Construct_UClass_UDeveloperSettings,
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_ToolsBox,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams UHT_STATICS::ClassParams = {
	&Z_Construct_UClass_UAssetAutoSaveSettings,
	"Editor",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	UHT_STATICS::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(UHT_STATICS::PropPointers),
	0,
	0x001000A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)
};
FClassRegistrationInfo Z_Registration_Info_UClass_UAssetAutoSaveSettings;
UClass* Z_Construct_UClass_UAssetAutoSaveSettings(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Inner)
	{
		using TClass = UAssetAutoSaveSettings;
		if (!Z_Registration_Info_UClass_UAssetAutoSaveSettings.InnerSingleton)
		{
			GetPrivateStaticClassBody(
				TClass::StaticPackage(),
				TEXT("AssetAutoSaveSettings"),
				Z_Registration_Info_UClass_UAssetAutoSaveSettings.InnerSingleton,
				nullptr,
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
		return Z_Registration_Info_UClass_UAssetAutoSaveSettings.InnerSingleton;
	}
	if (!Z_Registration_Info_UClass_UAssetAutoSaveSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAssetAutoSaveSettings.OuterSingleton, UHT_STATICS::ClassParams);
	}
	return Z_Registration_Info_UClass_UAssetAutoSaveSettings.OuterSingleton;
}
#undef UHT_STATICS
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UAssetAutoSaveSettings);
UAssetAutoSaveSettings::~UAssetAutoSaveSettings() {}
// ********** End Class UAssetAutoSaveSettings *****************************************************

// ********** Begin Registration *******************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h__Script_ToolsBox_Statics
struct UHT_STATICS
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ Z_Construct_UScriptStruct_FAssetImportRule, Z_Construct_UScriptStruct_FAssetImportRule_Statics::NewStructOps, TEXT("AssetImportRule"),&Z_Registration_Info_UScriptStruct_FAssetImportRule, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FAssetImportRule), 700485348U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UAssetAutoSaveSettings, TEXT("UAssetAutoSaveSettings"), &Z_Registration_Info_UClass_UAssetAutoSaveSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAssetAutoSaveSettings), 2788935148U) },
	};
}; // UHT_STATICS 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h__Script_ToolsBox_1473b90101f9a7a672fa58bfa905f3b6f6e2c847{
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
