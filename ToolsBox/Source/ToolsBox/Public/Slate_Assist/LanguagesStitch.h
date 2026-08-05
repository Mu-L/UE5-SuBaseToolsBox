#pragma once
#include "Containers/Array.h"
#include "Templates/SharedPointer.h"
#include "Widgets/Input/SComboBox.h" 

// 定义语言宏列表：枚举名, 显示名
#define LANGUAGE_LIST(f) \
f(Chinese, TEXT("中文")) \
f(English, TEXT("English")) \
f(Japanese, TEXT("日本語"))
 

enum class ELanguages : uint8
{
#define ENUM_GEN(name, display) name,
	LANGUAGE_LIST(ENUM_GEN)
#undef ENUM_GEN
};


class  LanguagesStitch
{
public:
	LanguagesStitch();
	TArray<TSharedPtr<FString>> LanguageOptions;

	
	TSharedPtr<FString> SelectedLanguage;

	TSharedPtr<SComboBox<TSharedPtr<FString>>> LanguageComboBox;

	static void SwitchLanguage(const FString& Language);

	static bool SetLanguageInUnplugin(const FString& NewLanguage);
	static FString GetLanguageFromUnplugin();
	
};
