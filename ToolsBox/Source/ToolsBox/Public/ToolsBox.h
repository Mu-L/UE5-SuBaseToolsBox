#pragma once


#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

class FUICommandList;

class FToolsBoxModule : public IModuleInterface
{
public:
	virtual void StartupModule() override; // 插件启动时的入口
	virtual void ShutdownModule() override; // 插件关闭时的扫尾

	void RegisterMenu(); 
	TSharedRef<SDockTab> OnSpawnToolsBoxTab(const FSpawnTabArgs& SpawnTabArgs); // 窗口怎么画
	void OnButtonClick();


	

	TSharedPtr<FUICommandList> OpenToolsBoxDockTab_CommandList; // 【插线板】把按钮点击映射到函数上


	//生成工具详情按钮
	//第二个参数传入的是要生成的对应的Tool面板类
	TSharedRef<SDockTab> OnSpawnToolTab(const FSpawnTabArgs& SpawnTabArgs, TFunction<TSharedRef<SWidget>()> NewToolTab);
	
};
