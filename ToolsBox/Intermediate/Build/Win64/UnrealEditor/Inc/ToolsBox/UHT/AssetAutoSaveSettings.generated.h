// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Tools/AssetImportAutoAllocation/AssetAutoSaveSettings.h"

#ifdef TOOLSBOX_AssetAutoSaveSettings_generated_h
#error "AssetAutoSaveSettings.generated.h already included, missing '#pragma once' in AssetAutoSaveSettings.h"
#endif
#define TOOLSBOX_AssetAutoSaveSettings_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin ScriptStruct FAssetImportRule **************************************************
struct Z_Construct_UScriptStruct_FAssetImportRule_Statics;
TOOLSBOX_API UScriptStruct* Z_Construct_UScriptStruct_FAssetImportRule(ETypeConstructPhase);

#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_15_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FAssetImportRule_Statics; \
	UE_NODEBUG static UScriptStruct* StaticStruct() { return Z_Construct_UScriptStruct_FAssetImportRule(ETypeConstructPhase::Inner); }


struct FAssetImportRule;
// ********** End ScriptStruct FAssetImportRule ****************************************************

// ********** Begin Class UAssetAutoSaveSettings ***************************************************
struct Z_Construct_UClass_UAssetAutoSaveSettings_Statics;
TOOLSBOX_API UClass* Z_Construct_UClass_UAssetAutoSaveSettings(ETypeConstructPhase);

#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_27_INCLASS_NO_PURE_DECLS \
private: \
	friend struct ::Z_Construct_UClass_UAssetAutoSaveSettings_Statics; \
	friend TOOLSBOX_API UClass* ::Z_Construct_UClass_UAssetAutoSaveSettings(ETypeConstructPhase); \
public: \
	DECLARE_CLASS2(UAssetAutoSaveSettings, UDeveloperSettings, COMPILED_IN_FLAGS(0 | CLASS_DefaultConfig | CLASS_Config), CASTCLASS_None, TEXT("/Script/ToolsBox"), Z_Construct_UClass_UAssetAutoSaveSettings) \
	DECLARE_SERIALIZER(UAssetAutoSaveSettings) \
	static constexpr const TCHAR* StaticConfigName() {return TEXT("Editor");} \



#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_27_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UAssetAutoSaveSettings(UAssetAutoSaveSettings&&) = delete; \
	UAssetAutoSaveSettings(const UAssetAutoSaveSettings&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAssetAutoSaveSettings); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAssetAutoSaveSettings); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UAssetAutoSaveSettings) \
	NO_API virtual ~UAssetAutoSaveSettings();


#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_24_PROLOG
#define FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_27_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_27_INCLASS_NO_PURE_DECLS \
	FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h_27_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UAssetAutoSaveSettings;

// ********** End Class UAssetAutoSaveSettings *****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UEWorkspace_UE5_8Project_SuBaseToolsBox_Plugins_ToolsBox_Source_ToolsBox_Public_Tools_AssetImportAutoAllocation_AssetAutoSaveSettings_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
