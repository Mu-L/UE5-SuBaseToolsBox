// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Tools/Right-ClickOperationTool/AssetAction.h"

#ifdef TOOLSBOX_AssetAction_generated_h
#error "AssetAction.generated.h already included, missing '#pragma once' in AssetAction.h"
#endif
#define TOOLSBOX_AssetAction_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPhysicalMaterial;
enum class ENaniteFallbackTarget : uint8;

// ********** Begin Class UAssetAction *************************************************************
#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execSimplifyMesh); \
	DECLARE_FUNCTION(execRemoveChar); \
	DECLARE_FUNCTION(execSetNaniteSetting); \
	DECLARE_FUNCTION(execSetMeshPhysicsMaterial); \
	DECLARE_FUNCTION(execRemoveUnusedVariables); \
	DECLARE_FUNCTION(execClearUnusedNodes); \
	DECLARE_FUNCTION(execSetConvexDecompositionCollision); \
	DECLARE_FUNCTION(execResizeTexture); \
	DECLARE_FUNCTION(execAddTags); \
	DECLARE_FUNCTION(execReplaceText_); \
	DECLARE_FUNCTION(execAutoAddPrefixAndSuffix); \
	DECLARE_FUNCTION(execAddPrefixAndSuffix);


struct Z_Construct_UClass_UAssetAction_Statics;
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAction(ETypeConstructPhase);

#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_INCLASS_NO_PURE_DECLS \
private: \
	friend struct ::Z_Construct_UClass_UAssetAction_Statics; \
	friend TOOLSBOX_API UClass* ::Z_Construct_UClass_UAssetAction(ETypeConstructPhase); \
public: \
	DECLARE_CLASS2(UAssetAction, UAssetActionUtility, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ToolsBox"), Z_Construct_UClass_UAssetAction) \
	DECLARE_SERIALIZER(UAssetAction)


#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UAssetAction(UAssetAction&&) = delete; \
	UAssetAction(const UAssetAction&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAssetAction); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAssetAction); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAssetAction) \
	NO_API virtual ~UAssetAction();


#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_15_PROLOG
#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_INCLASS_NO_PURE_DECLS \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UAssetAction;

// ********** End Class UAssetAction ***************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_Right_ClickOperationTool_AssetAction_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
