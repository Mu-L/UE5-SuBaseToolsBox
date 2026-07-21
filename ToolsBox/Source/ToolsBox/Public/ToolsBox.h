#pragma once


#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

class FUICommandList;

class FToolsBoxModule : public IModuleInterface
{
public:
	virtual void StartupModule() override; 
	virtual void ShutdownModule() override;

	void RegisterMenu(); 
	
	void OnButtonClick();


	TSharedRef<SDockTab> OnSpawnToolsBoxTab(const FSpawnTabArgs& SpawnTabArgs); 

	TSharedPtr<FUICommandList> OpenToolsBoxDockTab_CommandList; 


	//生成工具详情按钮
	//第二个参数传入的是要生成的对应的Tool面板类
	TSharedRef<SDockTab> OnSpawnToolTab(const FSpawnTabArgs& SpawnTabArgs, TFunction<TSharedRef<SWidget>()> NewToolTab);
	
};
