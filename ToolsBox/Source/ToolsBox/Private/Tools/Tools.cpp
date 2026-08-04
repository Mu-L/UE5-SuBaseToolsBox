#include "Tools/Tools.h"
#include "Internationalization/Internationalization.h"
#include "Templates/SharedPointer.h"
#include "Tools/AutoPrefix/AutoPrefix.h"

#include "Tools/BlankTemplateTool/BlankTemplateTool.h"
#include "Tools/IntelligentImportOfModelsAndMaterials/Import_MM.h"
#include "Tools/MaterialTttributeTransfer/MaterialTttributeTransfer.h"
#include "Tools/Right-ClickOperationTool/AAU.h"

#include "Tools/SpawnMaterial/SpawnMaterial.h"
#include "Tools/VariableCopier/VariableCopier.h"

#define LOCTEXT_NAMESPACE "Tools"

const TSet<FTool> Tools::Get_ToolsData()
{
	static TSet<FTool> InternalTools = {
		
		/*FTool(
			TEXT("空白模版"),                                       //标题
			LOCTEXT("ToolDetail", "开发者可复制此空白模版编写新工具"), //描述
			TEXT("ToolsBox.Image_Anon_1K"),						  //工具图标
			TEXT("SBlankTemplateToolTab"),						  //工具停靠栏名称
			TEXT("ToolsBox.Icon_Anon"),                           //停靠栏Icon
			[]() { return SNew(SBlankTemplateTool);}
			),*/
		
		FTool(
			TEXT("批量导入模型和材质"),
			LOCTEXT("ToolDetail", "批量导入模型和材质，同文件下的模型以及附属的纹理贴图会自动连接材质球并赋予模型"),
			TEXT("ToolsBox.Image_KitaIkuyo"),
			TEXT("SImport_MMTab"),
			TEXT("ToolsBox.Icon_Anon2"),
			TEXT("https://space.bilibili.com/391627131/"), 
			[]() { return SNew(SImport_MM);}
			),

		FTool(
			TEXT("右键菜单操作脚本"),
			LOCTEXT("ToolDetail", "右键菜单操作脚本"),
			TEXT("ToolsBox.Image_YamadaRyo"),
			TEXT("SAAUTab"),
			TEXT("ToolsBox.Icon_Anon2"),
			TEXT("https://space.bilibili.com/391627131/"), 
			[]() { return SNew(SAAU);}
			),

		FTool(
			TEXT("蓝图变量批量复制"),
			LOCTEXT("ToolDetail", "多选源蓝图的变量（函数，宏等），复制/剪切后一次性粘贴到另一个蓝图"),
			TEXT("ToolsBox.Image_Tomori"),
			TEXT("SVariableCopierTab"),
			TEXT("ToolsBox.Icon_Anon2"),
			TEXT("https://space.bilibili.com/391627131/"),
			[]() { return SNew(SVariableCopier);}
			),
		
		FTool(
			TEXT("批量材质球生成"),
			LOCTEXT("ToolDetail", "选择若干纹理生成材质球或材质实例并赋予模型"),
			TEXT("ToolsBox.Image_IjichiNijika"),
			TEXT("SSpawnMaterialTab"),
			TEXT("ToolsBox.Icon_Anon2"),
			TEXT("https://space.bilibili.com/391627131/"), 
			[]() { return SNew(SSpawnMaterial);}
			),
		
		FTool(
			TEXT("材质属性转移"),
			LOCTEXT("ToolDetail", "选择若干材质球或材质实例并通过变量命名将引用转移到新的材质球或材质实例"),
			TEXT("ToolsBox.Image_GotohHitori"),
			TEXT("SMaterialAttributeTransferTab"),
			TEXT("ToolsBox.Icon_Anon2"),
			TEXT("https://space.bilibili.com/391627131/"), 
			[]() { return SNew(SMaterialTttributeTransfer);}
			),
		FTool(
			TEXT("自动前缀"),
			LOCTEXT("ToolDetail", "常见新蓝图类时自动添加该类前缀"),
			TEXT("ToolsBox.Image_Mustumi"),
			TEXT("SAutoPrefixTab"),
			TEXT("ToolsBox.Icon_Anon2"),
			TEXT("https://space.bilibili.com/391627131/"), 
			[]() { return SNew(SAutoPrefix);}
			),
		

	};
	return InternalTools;
}


#undef LOCTEXT_NAMESPACE