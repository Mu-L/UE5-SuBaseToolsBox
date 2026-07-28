// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/Right-ClickOperationTool/ActorAction.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_UOBJECT");
void EmptyLinkFunctionForGeneratedCodeActorAction() {}

// ********** Begin Cross Module References ********************************************************
BLUTILITY_API UClass* Z_Construct_UClass_UActorActionUtility(ETypeConstructPhase);
// ********** End Cross Module References **********************************************************

// ********** Begin Same Module References *********************************************************
UPackage* Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UActorAction(ETypeConstructPhase);
TOOLSBOX_API UClass* Z_Construct_UClass_UActorAction(ETypeConstructPhase);
// ********** End Same Module References ***********************************************************
#define UHT_STRUCT_BASE(INIT) UE::CodeGen::ConstInit::TCompiledInObjectPtr<const FStructBaseChain>(UE::Private::AsStructBaseChain(INIT))

// ********** Begin Class UActorAction Function AlignSelectedActorsToGround ************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics
struct UHT_STATICS
{
	struct ActorAction_eventAlignSelectedActorsToGround_Parms
	{
		float GroundOffset;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
		{ "CallInEditor", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \n\x09 * \xe5\xb0\x86\xe9\x80\x89\xe4\xb8\xad\xe7\x9a\x84 Actors \xe5\xaf\xb9\xe9\xbd\x90\xe5\x88\xb0\xe5\x9c\xb0\xe9\x9d\xa2\xe5\xb9\xb6\xe6\xa0\xb9\xe6\x8d\xae\xe6\xb3\x95\xe7\xba\xbf\xe8\xb0\x83\xe6\x95\xb4\xe6\x97\x8b\xe8\xbd\xac\n\x09 */" },
#endif
		{ "DisplayName", "\xe5\x90\xb8\xe9\x99\x84\xe9\x80\x82\xe5\xba\x94\xe5\x9c\xb0\xe9\x9d\xa2" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/ActorAction.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xe5\xb0\x86\xe9\x80\x89\xe4\xb8\xad\xe7\x9a\x84 Actors \xe5\xaf\xb9\xe9\xbd\x90\xe5\x88\xb0\xe5\x9c\xb0\xe9\x9d\xa2\xe5\xb9\xb6\xe6\xa0\xb9\xe6\x8d\xae\xe6\xb3\x95\xe7\xba\xbf\xe8\xb0\x83\xe6\x95\xb4\xe6\x97\x8b\xe8\xbd\xac" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function AlignSelectedActorsToGround constinit property declarations ***********
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GroundOffset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function AlignSelectedActorsToGround constinit property declarations *************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function AlignSelectedActorsToGround Property Definitions **********************
const UECodeGen_Private::FFloatPropertyParams UHT_STATICS::NewProp_GroundOffset = { "GroundOffset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, nullptr, nullptr, 1, STRUCT_OFFSET(ActorAction_eventAlignSelectedActorsToGround_Parms, GroundOffset), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const UHT_STATICS::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&UHT_STATICS::NewProp_GroundOffset,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::PropPointers) < 2048);
// ********** End Function AlignSelectedActorsToGround Property Definitions ************************
const UECodeGen_Private::FFunctionParams UHT_STATICS::FuncParams = { { (FTypeConstructFunc*)Z_Construct_UClass_UActorAction, nullptr, "AlignSelectedActorsToGround", UHT_STATICS::PropPointers, UE_ARRAY_COUNT(UHT_STATICS::PropPointers), DataSizeOf<UHT_STATICS::ActorAction_eventAlignSelectedActorsToGround_Parms>(), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(UHT_STATICS::Type_MetaData), UHT_STATICS::Type_MetaData)},  };
static_assert(sizeof(UHT_STATICS::ActorAction_eventAlignSelectedActorsToGround_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround(ETypeConstructPhase Phase)
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, UHT_STATICS::FuncParams);
	}
	return ReturnFunction;
}
#undef UHT_STATICS
DEFINE_FUNCTION(UActorAction::execAlignSelectedActorsToGround)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_GroundOffset);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AlignSelectedActorsToGround(Z_Param_GroundOffset);
	P_NATIVE_END;
}
// ********** End Class UActorAction Function AlignSelectedActorsToGround **************************

// ********** Begin Class UActorAction *************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_Construct_UClass_UActorAction_Statics
struct UHT_STATICS
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Type_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Object" },
		{ "IncludePath", "Tools/Right-ClickOperationTool/ActorAction.h" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/ActorAction.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UActorAction constinit property declarations *****************************
// ********** End Class UActorAction constinit property declarations *******************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("AlignSelectedActorsToGround"), .Pointer = &UActorAction::execAlignSelectedActorsToGround },
	};
	static FTypeConstructFunc* DependentSingletons[];
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround, "AlignSelectedActorsToGround" }, // 6e714b338061cfb67bfafad699c70d364f856df1
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UActorAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct UHT_STATICS
FTypeConstructFunc* UHT_STATICS::DependentSingletons[] = {
	(FTypeConstructFunc*)Z_Construct_UClass_UActorActionUtility,
	(FTypeConstructFunc*)Z_Construct_UPackage__Script_ToolsBox,
};
static_assert(UE_ARRAY_COUNT(UHT_STATICS::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams UHT_STATICS::ClassParams = {
	&Z_Construct_UClass_UActorAction,
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
static void UActorAction_StaticRegisterNativesUActorAction()
{
	UClass* Class = UActorAction::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, 		MakeConstArrayView(UHT_STATICS::Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UActorAction;
UClass* Z_Construct_UClass_UActorAction(ETypeConstructPhase Phase)
{
	if (Phase == ETypeConstructPhase::Inner)
	{
		using TClass = UActorAction;
		if (!Z_Registration_Info_UClass_UActorAction.InnerSingleton)
		{
			GetPrivateStaticClassBody(
				TClass::StaticPackage(),
				TEXT("ActorAction"),
				Z_Registration_Info_UClass_UActorAction.InnerSingleton,
				UActorAction_StaticRegisterNativesUActorAction,
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
		return Z_Registration_Info_UClass_UActorAction.InnerSingleton;
	}
	if (!Z_Registration_Info_UClass_UActorAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UActorAction.OuterSingleton, UHT_STATICS::ClassParams);
	}
	return Z_Registration_Info_UClass_UActorAction.OuterSingleton;
}
#undef UHT_STATICS
UActorAction::UActorAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UActorAction);
UActorAction::~UActorAction() {}
// ********** End Class UActorAction ***************************************************************

// ********** Begin Registration *******************************************************************
#ifdef UHT_STATICS
#error UHT_STATICS already defined
#endif
#define UHT_STATICS Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h__Script_ToolsBox_Statics
struct UHT_STATICS
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UActorAction, TEXT("UActorAction"), &Z_Registration_Info_UClass_UActorAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UActorAction), 3270295382U) },
	};
}; // UHT_STATICS 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h__Script_ToolsBox_6b27cbad37919543016226ce8d291be6f129a93b{
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
