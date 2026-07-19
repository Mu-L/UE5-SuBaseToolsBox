#pragma once
 
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
 
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
 
	void Construct(const FArguments& InArgs);
 
private:
	FReply OnBrowseSourceClicked();
	FReply OnBrowseDestClicked();
	FReply OnStartImportClicked();
	FReply OnClearLog();
    
	// 日志辅助函数
	void AddLog(const FString& Message, FLinearColor Color);
    
	void ExecuteImportTask(const FImportFolderTask& Task, const FString& BaseDestPath);
 
	FString SourceFolderPath;
	FString RelativeDestPath;
 
	TSharedPtr<class SEditableTextBox> SourcePathBox;
	TSharedPtr<class SEditableTextBox> DestPathBox;
	TSharedPtr<class SMultiLineEditableText> LogBox;
	TSharedPtr<class SScrollBox> LogScrollBox;

	TSharedPtr<class FRichTextLayoutMarshaller> RichTextMarshaller;
};