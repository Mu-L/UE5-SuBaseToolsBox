// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Tools/Right-ClickOperationTool/ActorAction.h"

#ifdef TOOLSBOX_ActorAction_generated_h
#error "ActorAction.generated.h already included, missing '#pragma once' in ActorAction.h"
#endif
#define TOOLSBOX_ActorAction_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class UActorAction *************************************************************
#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execSetStaticMeshPivot); \
	DECLARE_FUNCTION(execAlignSelectedActorsToGround);


struct Z_Construct_UClass_UActorAction_Statics;
TOOLSBOX_API UClass* Z_Construct_UClass_UActorAction(ETypeConstructPhase);

#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_INCLASS_NO_PURE_DECLS \
private: \
	friend struct ::Z_Construct_UClass_UActorAction_Statics; \
	friend TOOLSBOX_API UClass* ::Z_Construct_UClass_UActorAction(ETypeConstructPhase); \
public: \
	DECLARE_CLASS2(UActorAction, UActorActionUtility, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ToolsBox"), Z_Construct_UClass_UActorAction) \
	DECLARE_SERIALIZER(UActorAction)


#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UActorAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UActorAction(UActorAction&&) = delete; \
	UActorAction(const UActorAction&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UActorAction); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UActorAction); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UActorAction) \
	NO_API virtual ~UActorAction();


#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_13_PROLOG
#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_INCLASS_NO_PURE_DECLS \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UActorAction;

// ********** End Class UActorAction ***************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_ActorAction_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
