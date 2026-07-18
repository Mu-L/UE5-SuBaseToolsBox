#include "Tools/Tools.h"
#include "Internationalization/Internationalization.h"
#include "Templates/SharedPointer.h"
#include "Tools/Cleaner/Cleaner.h"
#include "Tools/IntelligentImportOfModelsAndMaterials/Import_MM.h"

#define LOCTEXT_NAMESPACE "Tools"

const TSet<FTool> Tools::Get_ToolsData()
{
	static TSet<FTool> InternalTools = {
		
		FTool(
			TEXT("清理工具"),
			LOCTEXT("T1", "描述1..."),
			TEXT("ToolsBox.Image_Anon_254px"),
			TEXT("CleanerTab"),
			[]() { return SNew(SCleaner);}
			),
		
		FTool(
			TEXT("批量导入模型和材质"),
			LOCTEXT("T2", "批量导入模型和材质，同文件下的模型以及附属的纹理贴图会自动连接材质球并赋予模型"),
			TEXT("ToolsBox.Image_Anon_254px"),
			TEXT("SImport_MMTab"),
			[]() { return SNew(SImport_MM);}
			)
				
		
	};
	return InternalTools;
}


#undef LOCTEXT_NAMESPACE