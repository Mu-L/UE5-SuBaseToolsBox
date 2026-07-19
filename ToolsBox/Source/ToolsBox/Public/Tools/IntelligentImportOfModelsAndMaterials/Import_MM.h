#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

// 前向声明，优化编译速度
class SEditableTextBox;
class SMultiLineEditableText;
class SScrollBox;
class FRichTextLayoutMarshaller;

struct FImportFolderTask
{
	FString FolderName;
	FString MeshPath;
	TMap<FString, FString> TextureMap;
};

class SImport_MM : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImport_MM) {}
	SLATE_END_ARGS()

	/** 构建 Slate 界面 */
	void Construct(const FArguments& InArgs);

private:
	/** UI 事件回调 */
	FReply OnBrowseSourceClicked();
	FReply OnBrowseDestClicked();
	FReply OnStartImportClicked();
	FReply OnClearLog();
    
	/** 
	 * 向日志框添加信息
	 * @param Message 日志内容
	 * @param Color   颜色（支持富文本渲染）
	 */
	void AddLog(const FString& Message, FLinearColor Color);
    
	/** 执行具体的 FBX/OBJ 导入逻辑 */
	void ExecuteImportTask(const FImportFolderTask& Task, const FString& BaseDestPath);

	/** 路径数据 */
	FString SourceFolderPath;
	FString RelativeDestPath;

	/** UI 组件指针 */
	TSharedPtr<SEditableTextBox> SourcePathBox;
	TSharedPtr<SEditableTextBox> DestPathBox;
	TSharedPtr<SEditableTextBox> TexSubFolderNameBox; // 已修正为单行文本框指针
    
	TSharedPtr<SMultiLineEditableText> LogBox;
	TSharedPtr<SScrollBox> LogScrollBox;

	/** 
	 * 富文本处理器：用于在 SMultiLineEditableText 中显示颜色标签 
	 * 命名已修改为 MarshallerPtr 以避免隐藏局部变量 (C4458)
	 */
	TSharedPtr<FRichTextLayoutMarshaller> RichTextMarshallerPtr;
};
