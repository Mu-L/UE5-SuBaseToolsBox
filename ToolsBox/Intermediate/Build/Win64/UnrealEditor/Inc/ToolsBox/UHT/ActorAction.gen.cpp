// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/Right-ClickOperationTool/ActorAction.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeActorAction() {}

// ********** Begin Cross Module References ********************************************************
BLUTILITY_API UClass* Z_Construct_UClass_UActorActionUtility();
TOOLSBOX_API UClass* Z_Construct_UClass_UActorAction();
TOOLSBOX_API UClass* Z_Construct_UClass_UActorAction_NoRegister();
UPackage* Z_Construct_UPackage__Script_ToolsBox();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UActorAction Function AlignSelectedActorsToGround ************************
struct Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics
{
	struct ActorAction_eventAlignSelectedActorsToGround_Parms
	{
		float GroundOffset;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
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
	static const UECodeGen_Private::FFloatPropertyParams NewProp_GroundOffset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::NewProp_GroundOffset = { "GroundOffset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ActorAction_eventAlignSelectedActorsToGround_Parms, GroundOffset), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::NewProp_GroundOffset,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UActorAction, nullptr, "AlignSelectedActorsToGround", Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::PropPointers), sizeof(Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::ActorAction_eventAlignSelectedActorsToGround_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::Function_MetaDataParams), Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::ActorAction_eventAlignSelectedActorsToGround_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround_Statics::FuncParams);
	}
	return ReturnFunction;
}
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
void UActorAction::StaticRegisterNativesUActorAction()
{
	UClass* Class = UActorAction::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AlignSelectedActorsToGround", &UActorAction::execAlignSelectedActorsToGround },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UActorAction;
UClass* UActorAction::GetPrivateStaticClass()
{
	using TClass = UActorAction;
	if (!Z_Registration_Info_UClass_UActorAction.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("ActorAction"),
			Z_Registration_Info_UClass_UActorAction.InnerSingleton,
			StaticRegisterNativesUActorAction,
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
	return Z_Registration_Info_UClass_UActorAction.InnerSingleton;
}
UClass* Z_Construct_UClass_UActorAction_NoRegister()
{
	return UActorAction::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UActorAction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Object" },
		{ "IncludePath", "Tools/Right-ClickOperationTool/ActorAction.h" },
		{ "ModuleRelativePath", "Public/Tools/Right-ClickOperationTool/ActorAction.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UActorAction_AlignSelectedActorsToGround, "AlignSelectedActorsToGround" }, // 952708982
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UActorAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UActorAction_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorActionUtility,
	(UObject* (*)())Z_Construct_UPackage__Script_ToolsBox,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UActorAction_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UActorAction_Statics::ClassParams = {
	&UActorAction::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UActorAction_Statics::Class_MetaDataParams), Z_Construct_UClass_UActorAction_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UActorAction()
{
	if (!Z_Registration_Info_UClass_UActorAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UActorAction.OuterSingleton, Z_Construct_UClass_UActorAction_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UActorAction.OuterSingleton;
}
UActorAction::UActorAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UActorAction);
UActorAction::~UActorAction() {}
// ********** End Class UActorAction ***************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h__Script_ToolsBox_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UActorAction, UActorAction::StaticClass, TEXT("UActorAction"), &Z_Registration_Info_UClass_UActorAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UActorAction), 1892138414U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h__Script_ToolsBox_3362715267(TEXT("/Script/ToolsBox"),
	Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h__Script_ToolsBox_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UEWorkspace_UE5_6Project_toolsbox56_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h__Script_ToolsBox_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
