#include "Tools/Tools.h"
#include "Internationalization/Internationalization.h"
#include "Templates/SharedPointer.h"
#include "Tools/Cleaner/Cleaner.h"

#define LOCTEXT_NAMESPACE "Tools"

const TSet<FTool> Tools::Get_ToolsData()
{
	static TSet<FTool> InternalTools = {
		
		FTool(
			TEXT("清理工具"),
			LOCTEXT("D1", "描述1..."),
			TEXT("ToolsBox.Image_Anon_254px"),
			TEXT("CleanerTab"),
			[]() { return SNew(SCleaner);}
			),
		
		FTool(
			TEXT("转换工具"),
			LOCTEXT("D2", "描述2..."),
			TEXT("ToolsBox.Image_Anon_254px"),
			TEXT("ConverterTab"),
			[]() { return SNew(SCleaner);}
			)
				
		
	};
	return InternalTools;
}


#undef LOCTEXT_NAMESPACE