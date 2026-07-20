#include "ToolsBox.h"
#include "ToolMenus.h"
#include "OpenToolsBox_Command.h"
#include "Framework/Commands/UICommandList.h" // 必须包含这个才能用 MapAction
#include "Slate_Assist/FIconStyle.h"
#include "Slate_Assist/SlateAssistBuildFunctionLibrary.h"
#include "Tools/Tools.h"
#include "Tools/Cleaner/Cleaner.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSearchBox.h"
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
    // 定义列表容器。
    // 注意：这里必须是 TSharedPtr，因为 Lambda 需要捕获它。
    TSharedPtr<SVerticalBox> ListContainer;
 
    // 提前 AssignNew，这样我们在定义 RefreshList 时 ListContainer 已经有值了
    SAssignNew(ListContainer, SVerticalBox);
 
    // 【重要修复】：RefreshList 必须捕获 ListContainer 的拷贝 (TSharedPtr)
    // 之前使用 [&ListContainer] 捕获的是局部变量的地址，函数执行完该地址就失效了，导致输入时崩溃。
    auto RefreshList = [ListContainer](FString SearchText = TEXT(""))
    {
        if (!ListContainer.IsValid()) return;
 
        ListContainer->ClearChildren();
 
        for (const auto& Tool : Tools::Get_ToolsData())
        {
            FString NameStr = Tool.ToolName.ToString();
            FString DescStr = Tool.ToolDescription.ToString();
 
            if (SearchText.IsEmpty() || NameStr.Contains(SearchText) || DescStr.Contains(SearchText))
            {
                ListContainer->AddSlot()
                .AutoHeight()
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
    };
 
    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).TabRole(ETabRole::NomadTab)
    [
        SNew(SVerticalBox)
 
        // 1. 顶部搜索栏
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(10.0f, 10.0f, 10.0f, 5.0f)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            .VAlign(VAlign_Center)
            [
                SNew(SSearchBox)
                .HintText(NSLOCTEXT("ToolsBox", "SearchHint", "输入关键词搜索工具..."))
                // 这里捕获 RefreshList 这个 lambda。RefreshList 内部已经安全持有 ListContainer。
                .OnTextChanged_Lambda([RefreshList](const FText& InText) {
                    RefreshList(InText.ToString()); 
                })
            ]
 
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(8.0f, 0, 0, 0)
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "SimpleButton")
                .OnClicked_Lambda([]() { return FReply::Handled(); })
                [
                    SNew(SImage)
                    .Image(FAppStyle::GetBrush("Icons.Info"))
                    .DesiredSizeOverride(FVector2D(18, 18))
                ]
            ]
        ]
 
        // 2. 下方滚动列表
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .Padding(10.0f, 5.0f, 10.0f, 10.0f)
        [
            SNew(SScrollBox)
            + SScrollBox::Slot()
            [
                ListContainer.ToSharedRef() // 直接放入预先创建好的容器
            ]
        ]
    ];
 
    // 初始化加载
    RefreshList();
 
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