// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWStructTypes.h"
#include "Misc/ConfigCacheIni.h"

/**
 * 
 */
class SOW_API MapScriptLoader
{
public:
	static bool LoadFromFile(const FString& AbsPath, FLoadedMapSpec& OutSpec, FString& OutError);

private:
	static bool ParseMeta(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err);
	static bool ParseClassMap(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err);
	static bool ParseGrid(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err);

	static void SplitCSV(const FString& Line, TArray<FString>& OutTokens);
	static bool GetStringFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, FString& Out);
	static bool GetIntFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, int32& Out);
	static bool GetFloatFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, float& Out);
};
