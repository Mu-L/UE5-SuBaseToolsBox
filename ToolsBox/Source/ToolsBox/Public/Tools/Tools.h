#pragma once

#define LOCTEXT_NAMESPACE "Tools"
#include "Internationalization/Text.h"
#include "UObject/NameTypes.h"
#include "Widgets/SWidget.h"

struct FTool
{

	FName ToolName;
	FText ToolDescription;
	FName ToolImage;
	FName ToolTabID; 
    

	TFunction<TSharedRef<SWidget>()> WidgetFactory;
 
	FTool(FName InName, const FText& InDesc, FName InImage, FName InTabID, TFunction<TSharedRef<SWidget>()> InFactory)
		: ToolName(InName), ToolDescription(InDesc), ToolImage(InImage), ToolTabID(InTabID), WidgetFactory(InFactory) {}
	
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