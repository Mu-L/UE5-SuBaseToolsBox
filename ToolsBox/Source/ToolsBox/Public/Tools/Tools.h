#pragma once


#include "Internationalization/Text.h"
#include "UObject/NameTypes.h"
#include "Widgets/SWidget.h"



#define LOCTEXT_NAMESPACE "Tools"
struct FTool
{

	FName ToolName;
	FText ToolDescription;
	FName ToolImage;
	FName ToolTabID;
	FName ToolDockTabIcon;
	FString ToolURL; // 工具对应的帮助/说明页面链接（点击右上角图标按钮打开）
    

	TFunction<TSharedRef<SWidget>()> WidgetFactory;
 
	FTool(FName InName, const FText& InDesc, FName InImage, FName InTabID, FName Icon, FString InURL, TFunction<TSharedRef<SWidget>()> InFactory)
		: ToolName(InName), ToolDescription(InDesc), ToolImage(InImage), ToolTabID(InTabID), ToolDockTabIcon(Icon), ToolURL(InURL), WidgetFactory(InFactory) {}
	
	bool operator==(const FTool& Other) const
	{
		return ToolName == Other.ToolName;
	}
};


FORCEINLINE uint32 GetTypeHash(const FTool& Thing)
{
	return GetTypeHash(Thing.ToolName);
}


class Tools
{

	
public:

	static const TSet<FTool> Get_ToolsData();

	
};


#undef LOCTEXT_NAMESPACE