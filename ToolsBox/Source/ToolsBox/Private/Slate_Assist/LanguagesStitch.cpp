#include "Slate_Assist/LanguagesStitch.h"

#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "Serialization/JsonWriter.h"


LanguagesStitch::LanguagesStitch()
{
	
	// 自动生成填充逻辑
	#define ARRAY_GEN(name, display) LanguageOptions.Add(MakeShared<FString>(display));
		LANGUAGE_LIST(ARRAY_GEN)
	#undef ARRAY_GEN
	SelectedLanguage = LanguageOptions[0];
}

void LanguagesStitch::SwitchLanguage(const FString& Language)
{
	SetLanguageInUnplugin(Language);
	//加载语言文件
}

bool LanguagesStitch::SetLanguageInUnplugin(const FString& NewLanguage)
{
	FString JsonString;
	// 1. 先读出当前内容，确保不破坏文件其他结构
	if (!FFileHelper::LoadFileToString(JsonString, *FPaths::ProjectPluginsDir()))
	{
		return false;
	}
 
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
 
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// 2. 更新或添加字段
		JsonObject->SetStringField(TEXT("Language"), NewLanguage);
 
		// 3. 序列化回字符串
		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
		{
			// 4. 写回文件
			return FFileHelper::SaveStringToFile(OutputString,*FPaths::ProjectPluginsDir());
		}
	}
 
	return false;
}

FString LanguagesStitch::GetLanguageFromUnplugin()
{
	FString JsonString;
	// 1. 读取文件到字符串
	if (!FFileHelper::LoadFileToString(JsonString, *FPaths::ProjectPluginsDir()))
	{
		UE_LOG(LogTemp, Error, TEXT("无法读取文件: %s"),*FPaths::ProjectPluginsDir());
		return TEXT("");
	}
 
	// 2. 解析 JSON
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
 
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// 3. 尝试读取 Language 字段
		FString OutLanguage;
		if (JsonObject->TryGetStringField(TEXT("Language"), OutLanguage))
		{
			return OutLanguage;
		}
	}
 
	return TEXT("");
}


