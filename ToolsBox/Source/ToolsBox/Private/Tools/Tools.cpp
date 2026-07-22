#include "Tools/Tools.h"
#include "Internationalization/Internationalization.h"
#include "Templates/SharedPointer.h"
#include "Tools/AssetActionUtility/AAU.h"
#include "Tools/BlankTemplateTool/BlankTemplateTool.h"
#include "Tools/IntelligentImportOfModelsAndMaterials/Import_MM.h"

#define LOCTEXT_NAMESPACE "Tools"

const TSet<FTool> Tools::Get_ToolsData()
{
	static TSet<FTool> InternalTools = {
		
		FTool(
			TEXT("空白模版"),//标题
			LOCTEXT("ToolDetail", "开发者可复制此空白模版编写新工具"),//描述
			TEXT("ToolsBox.Image_Anon_1K"),
			TEXT("SBlankTemplateToolTab"),
			TEXT("ToolsBox.Icon_Anon"),//停靠栏Icon
			[]() { return SNew(SBlankTemplateTool);}
			),
		
		FTool(
			TEXT("批量导入模型和材质"),
			LOCTEXT("ToolDetail", "批量导入模型和材质，同文件下的模型以及附属的纹理贴图会自动连接材质球并赋予模型"),
			TEXT("ToolsBox.Image_Anon_1K"),
			TEXT("SImport_MMTab"),
			TEXT("ToolsBox.Icon_Anon"),
			[]() { return SNew(SImport_MM);}
			),

		FTool(
			TEXT("资产操作工具"),
			LOCTEXT("AssetUtility", "编辑器资产工具"),
			TEXT("ToolsBox.Image_Anon_1K"),
			TEXT("SAAUTab"),
			TEXT("ToolsBox.Icon_Anon"),
			[]() { return SNew(SAAU);}
			)
		
				
		
	};
	return InternalTools;
}


#undef LOCTEXT_NAMESPACE