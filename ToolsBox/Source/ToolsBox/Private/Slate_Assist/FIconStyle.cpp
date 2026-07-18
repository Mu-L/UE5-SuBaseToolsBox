#include "Slate_Assist/FIconStyle.h"

#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"

TSharedPtr<FSlateStyleSet> FIconStyle::Icons = nullptr;
TSharedPtr<FSlateStyleSet> FIconStyle::Images = nullptr;
 
void FIconStyle::Initialize()
{
	FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("ToolsBox"))->GetBaseDir() / TEXT("Resources");
 
	// 优先初始化 Images
	if (!Images.IsValid())
	{
		Images = MakeShareable(new FSlateStyleSet("ToolsBoxDockTabStyle"));
		Images->SetContentRoot(ContentDir);
		Images->Set("ToolsBox.Image_Anon_254px",
			new FSlateImageBrush(Images->RootToContentDir(TEXT("Icon_Anon"), TEXT(".png")), FVector2D(254.0f, 254.0f)));
		FSlateStyleRegistry::RegisterSlateStyle(*Images);
	}
 
	// 后初始化 Icons
	if (!Icons.IsValid())
	{
		Icons = MakeShareable(new FSlateStyleSet("EditorToolsBoxIconStyle"));
		Icons->SetContentRoot(ContentDir);

		// 【关键修改点】
		// 这个 Key 的名字必须是 "Context名.变量名"
		// 对应 FOpenToolsBox_Command 里的定义
		Icons->Set("ToolsBox.ToolsBox_CommandInfo",// 如果图标要作用在按钮上命名规则为  [按钮的ID名，如TEXT("ToolsBox")] . [按钮指针变量名，如TSharedPtr<FUICommandInfo> ToolsBox_CommandInfo]
			new FSlateImageBrush(Icons->RootToContentDir(TEXT("Icon_SuBaRu"), TEXT(".png")), FVector2D(20.0f, 20.0f)));

		
		// 修正：使用 Icons 自己的指针去拼路径，不要跨变量引用
		Icons->Set("ToolsBox.Icon_Anon",
			new FSlateImageBrush(Icons->RootToContentDir(TEXT("Icon_Anon"), TEXT(".png")), FVector2D(20.0f, 20.0f)));
 
		FSlateStyleRegistry::RegisterSlateStyle(*Icons);
	}
}
 
void FIconStyle::Shutdown()
{
	// 消除 (Eliminate) 注册信息，清空内存
	FSlateStyleRegistry::UnRegisterSlateStyle(*Icons);
	FSlateStyleRegistry::UnRegisterSlateStyle(*Images);
	Icons.Reset();
	Images.Reset();
}
 
FName FIconStyle::Get_IconsName() { return Icons->GetStyleSetName(); }

FName FIconStyle::Get_ImagesName(){ return Images->GetStyleSetName();}

const ISlateStyle& FIconStyle::Get_Icons() { return *Icons; }

const ISlateStyle& FIconStyle::Get_Images(){ return *Images;}
