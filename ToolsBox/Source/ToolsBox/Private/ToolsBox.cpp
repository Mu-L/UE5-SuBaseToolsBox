#include "ToolsBox.h"
#include "ToolMenus.h"
#include "OpenToolsBox_Command.h"
#include "Framework/Commands/UICommandList.h" // 必须包含这个才能用 MapAction
#include "Slate_Assist/FIconStyle.h"
#include "Slate_Assist/SlateAssistBuildFunctionLibrary.h"
#include "Tools/Tools.h"
#include "Tools/Cleaner/Cleaner.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"

#define LOCTEXT_NAMESPACE "FToolsBoxModule"

void FToolsBoxModule::StartupModule()
{
	// 1. 开启化妆间（加载图标）
	FIconStyle::Initialize();
	
	// 2. 登记身份证（注册按钮名）
	FOpenToolsBox_Command::Register();

	// 3. 准备插线板，并把按钮和 OnButtonClick 函数连接起来
	OpenToolsBoxDockTab_CommandList = MakeShareable(new FUICommandList);
	OpenToolsBoxDockTab_CommandList->MapAction
	(
		FOpenToolsBox_Command::Get().ToolsBox_CommandInfo,
		FExecuteAction::CreateRaw(this, &FToolsBoxModule::OnButtonClick),
		FCanExecuteAction()
	);

	// 1. 注册主工具箱
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner("ToolsBox", FOnSpawnTab::CreateRaw(this, &FToolsBoxModule::OnSpawnToolsBoxTab))
		.SetDisplayName(LOCTEXT("ToolsBox", "工具箱"))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FIconStyle::Get_IconsName(), "ToolsBox.Icon_Anon"));
 
	// 2. 在启动时一次性注册所有子工具窗口
	for (const FTool& Tool : Tools::Get_ToolsData())
	{
		FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
			Tool.ToolTabID, 
			FOnSpawnTab::CreateRaw(this, &FToolsBoxModule::OnSpawnToolTab, Tool.WidgetFactory) // 绑定 Payload 参数
		)
		.SetDisplayName(FText::FromName(Tool.ToolName))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	}
 
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FToolsBoxModule::RegisterMenu));
}


//这个函数负责将按钮渲染到对应位置
void FToolsBoxModule::RegisterMenu()
{
	// 锁定修改权
	FToolMenuOwnerScoped OwnerScoped(this);
	
	// 找到工具栏上放“运行”按钮那一横行
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	
	// 开辟一个小分组
	FToolMenuSection& Section = Menu->FindOrAddSection("SuBase_LevelEditorToolBar");
	
	// 把按钮画上去
	FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FOpenToolsBox_Command::Get().ToolsBox_CommandInfo));
	
	// 【重点】把插线板给按钮，点击才有反应！
	Entry.SetCommandList(OpenToolsBoxDockTab_CommandList);
}

TSharedRef<SDockTab> FToolsBoxModule::OnSpawnToolsBoxTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<SWrapBox> WrapBox;
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot().Padding(15.0f)
		[
			SAssignNew(WrapBox, SWrapBox).UseAllottedSize(true).InnerSlotPadding(FVector2D(15.0f, 15.0f))
		]
	];
 
	if (WrapBox.IsValid())
	{
		// 渲染 UI 时不再进行注册操作，只负责画按钮
		for (const auto& Tool : Tools::Get_ToolsData())
		{
			WrapBox->AddSlot()
			[
				SlateAssistBuildFunctionLibrary::MakeToolBlock(
					FText::FromName(Tool.ToolName),
					Tool.ToolDescription,
					Tool.ToolImage,
					Tool.ToolTabID
				)
			];
		}
	}
	return SpawnedTab;
}

void FToolsBoxModule::OnButtonClick()
{
	// 打开工具箱
	FGlobalTabmanager::Get()->TryInvokeTab(FTabId("ToolsBox"));
}

TSharedRef<SDockTab> FToolsBoxModule::OnSpawnToolTab(const FSpawnTabArgs& SpawnTabArgs, TFunction<TSharedRef<SWidget>()> NewToolTab)
{
	
	TSharedRef<SWidget> Content = NewToolTab ? NewToolTab() : SNullWidget::NullWidget;
 
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			Content
		];
}

void FToolsBoxModule::ShutdownModule()
{
	
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("ToolsBox");
	FOpenToolsBox_Command::Unregister();
	FIconStyle::Shutdown();
	for (const FTool& Tool : Tools::Get_ToolsData())
	{
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(Tool.ToolTabID);
	}
    
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("ToolsBox");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FToolsBoxModule, ToolsBox)