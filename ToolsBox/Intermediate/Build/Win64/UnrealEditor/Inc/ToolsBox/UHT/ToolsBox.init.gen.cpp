// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeToolsBox_init() {}
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_ToolsBox;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_ToolsBox(ETypeConstructPhase)
	{
		if (!Z_Registration_Info_UPackage__Script_ToolsBox.OuterSingleton)
		{
		static const UECodeGen_Private::FPackageParams PackageParams = {
			"/Script/ToolsBox",
			nullptr,
			0,
			PKG_CompiledIn | 0x00000040,
			0x6740EED6,
			0x86F6BBE8,
			METADATA_PARAMS(0, nullptr)
		};
		UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_ToolsBox.OuterSingleton, PackageParams);
	}
	return Z_Registration_Info_UPackage__Script_ToolsBox.OuterSingleton;
}
static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_ToolsBox(Z_Construct_UPackage__Script_ToolsBox, TEXT("/Script/ToolsBox"), Z_Registration_Info_UPackage__Script_ToolsBox, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x6740EED6, 0x86F6BBE8));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
