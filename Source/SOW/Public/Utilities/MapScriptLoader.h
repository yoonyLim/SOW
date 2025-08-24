// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"

struct FEnemyRoute
{
	FString RouteName;
	TArray<FIntPoint> Points;
};

struct FLoadedMapSpec
{
	int32 GridWidth = 0;

	int32 GridHeight = 0;

	float TileWidth = 100.f;

	float TileHeight = 100.f;

	FName DefaultToken = NAME_None;

	//Token -> Soft Class
	TMap<FName, TSoftClassPtr<AActor>> TokenToClass;

	// Token Table
	TArray<TArray<FName>> GridTokens;

	TArray<FEnemyRoute> EnemyRoutes;

public:
	bool IsValid() const
	{
		return GridWidth > 0 && GridHeight > 0
			&& TileWidth > 0.f && TileHeight > 0.f
			&& GridTokens.Num() == GridHeight
			&& (GridTokens.IsEmpty() || GridTokens[0].Num() == GridWidth);
	}
};

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
	static bool ParseRoutes(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err);

	static void SplitCSV(const FString& Line, TArray<FString>& OutTokens);
	static bool GetStringFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, FString& Out);
	static bool GetIntFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, int32& Out);
	static bool GetFloatFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, float& Out);
};
