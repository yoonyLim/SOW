// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/MapScriptLoader.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/ConfigCacheIni.h"

void MapScriptLoader::SplitCSV(const FString& Line, TArray<FString>& OutTokens)
{
	OutTokens.Reset();
	Line.ParseIntoArray(OutTokens, TEXT(","), true);
	for (FString& T : OutTokens) { T = T.TrimStartAndEnd(); }
}

static const FConfigSection* FindSection(const FConfigFile& Cfg, const TCHAR* Section)
{
	return Cfg.FindSection(Section);
}

bool MapScriptLoader::GetStringFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, FString& Out)
{
	const FConfigSection* S = FindSection(Cfg, Section);
	if (!S) return false;

	const FConfigValue* V = S->Find(Key);
	if (!V) return false;

	Out = V->GetValue();
	return true;
}

bool MapScriptLoader::GetIntFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, int32& Out)
{
	FString Str;
	if (!GetStringFromSection(Cfg, Section, Key, Str)) return false;
	Out = FCString::Atoi(*Str);
	return true;
}

bool MapScriptLoader::GetFloatFromSection(const FConfigFile& Cfg, const TCHAR* Section, const TCHAR* Key, float& Out)
{
	FString Str;
	if (!GetStringFromSection(Cfg, Section, Key, Str)) return false;
	Out = FCString::Atoi(*Str);
	return true;
}

bool MapScriptLoader::LoadFromFile(const FString& AbsPath, FLoadedMapSpec& Out, FString& OutError)
{
	FString Raw;
	if (!FFileHelper::LoadFileToString(Raw, *AbsPath))
	{
		OutError = FString::Printf(TEXT("MapScriptLoader - Cannot reaed file: %s"), *AbsPath);
		return false;
	}

	FConfigFile Cfg;
	Cfg.ProcessInputFileContents(Raw, *AbsPath);

	if (!ParseMeta(Cfg, Out, OutError)) return false;
	if (!ParseClassMap(Cfg, Out, OutError)) return false;
	if (!ParseGrid(Cfg, Out, OutError)) return false;
	if (!ParseRoutes(Cfg, Out, OutError)) return false;

	return true;
}

bool MapScriptLoader::ParseMeta(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err)
{
	if (!FindSection(Cfg, TEXT("Meta")))
	{
		Err = TEXT("MapScriptLoader: [Meta] section not found");
		return false;
	}
	if (!GetIntFromSection(Cfg, TEXT("Meta"), TEXT("GridWidth"), Out.GridWidth) ||
		!GetIntFromSection(Cfg, TEXT("Meta"), TEXT("GridHeight"), Out.GridHeight))
	{
		Err = TEXT("MapScriptLoader: Meta.GridWidth / Meta.GridHeight missing or invaild");
		return false;
	}
	if (!GetFloatFromSection(Cfg, TEXT("Meta"), TEXT("TileWidth"), Out.TileWidth) ||
		!GetFloatFromSection(Cfg, TEXT("Meta"), TEXT("TileHeight"), Out.TileHeight))
	{
		Err = TEXT("MapScriptLoader: Meta.TileWidth / Meta.TileHeight missing or invaild");
		return false;
	}

	FString DefaultTk;
	if (!GetStringFromSection(Cfg, TEXT("Meta"), TEXT("Default"), DefaultTk) || DefaultTk.IsEmpty())
	{
		Err = TEXT("MapScriptLoader: Meta.Default missing");
		return false;
	}
	Out.DefaultToken = FName(*DefaultTk);

	return true;
}

bool MapScriptLoader::ParseClassMap(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err)
{
	const FConfigSection* S = FindSection(Cfg, TEXT("ClassMap"));
	if (!S)
	{
		Err = TEXT("MapScriptLoader: [ClassMap] section not found");
		return false;
	}

	for (const auto& Pair : *S)
	{
		const FName Token = *Pair.Key.GetPlainNameString();
		const FString Path = Pair.Value.GetValue();
		if (Path.IsEmpty()) {
			continue;
		}
		Out.TokenToClass.Add(Token, TSoftClassPtr<AActor>(Path));
	}
	return true;
}

bool MapScriptLoader::ParseGrid(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err)
{
	const FConfigSection* S = FindSection(Cfg, TEXT("Grid"));
	if (!S)
	{
		Err = TEXT("MapScriptLoader: [Grid] section not found");
		return false;
	}

	Out.GridTokens.SetNum(Out.GridHeight);

	for (int32 Y = 0; Y < Out.GridHeight; ++Y)
	{
		const FString Key = FString::Printf(TEXT("Row%d"), Y);

		const FConfigValue* V = S->Find(*Key);
		if (!V)
		{
			Err = FString::Printf(TEXT("MapScriptLoader: Missing [Grid].%s"), *Key);
			return false;
		}

		const FString Line = V->GetValue();

		TArray<FString> Cells;
		SplitCSV(Line, Cells);

		if (Cells.Num() != Out.GridWidth)
		{
			Err = FString::Printf(TEXT("MapScriptLoader: [Grid].%s must have %d tokens, got $d"), *Key, Out.GridWidth, Cells.Num());
			return false;
		}

		Out.GridTokens[Y].SetNum(Out.GridWidth);
		for (int32 X = 0; X < Out.GridWidth; ++X)
		{
			Out.GridTokens[Y][X] = FName(*Cells[X]);
		}
	}

	return true;
}

/* Parse Routes */
static void NormalizeRouteLine(FString& S)
{
	/*
		In .ini,
		ex)
		[Route Name] = [Coord Sets] 
		MainPath = (0, 0) -> (0, 5) -> (3, 5)
	*/

	S.ReplaceInline(TEXT(" "), TEXT(""));
	S.ReplaceInline(TEXT("("), TEXT(""));
	S.ReplaceInline(TEXT(")"), TEXT(""));
	S.ReplaceInline(TEXT("->"), TEXT(";"), ESearchCase::IgnoreCase);
}

bool MapScriptLoader::ParseRoutes(const FConfigFile& Cfg, FLoadedMapSpec& Out, FString& Err)
{
	const FConfigSection* S = FindSection(Cfg, TEXT("EnemyRoutes"));
	if (!S)
	{
		Out.EnemyRoutes.Reset();
		return true;
	}

	for (const auto& Pair : *S)
	{
		FEnemyRoute R;
		R.RouteName = Pair.Key.GetPlainNameString();

		FString Line = Pair.Value.GetValue();
		NormalizeRouteLine(Line);

		TArray<FString> Segs;
		Line.ParseIntoArray(Segs, TEXT(";"), true);

		if (Segs.Num() < 2)
		{
			continue;
		}

		for (const FString& Seg : Segs)
		{
			TArray<FString> XY;
			Seg.ParseIntoArray(XY, TEXT(","), true);

			if (XY.Num() != 2) { Err = FString::Printf(TEXT("MapScriptLoader: [EnemyRoutes].%s : malformed '%s'"), *R.RouteName, *Seg);  return false; }

			const int32 X = FCString::Atoi(*XY[0]);
			const int32 Y = FCString::Atoi(*XY[1]);

			if (X < 0 || X >= Out.GridWidth || Y < 0 || Y >= Out.GridHeight)
			{
				Err = FString::Printf(TEXT("MapScriptLoader: [EnemyRoutes].%s : point (%d, %d) out of bounds [%d x %d]"),
					*R.RouteName, X, Y, Out.GridWidth, Out.GridHeight);
				return false;
			}
			R.Points.Add(FIntPoint(X, Y));
		}

		Out.EnemyRoutes.Add(MoveTemp(R));
	}

	return true;
}