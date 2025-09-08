// Fill out your copyright notice in the Description page of Project Settings.


#include "Log/CurrencyGainLogger.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Log/CurrencyLog.h"

void UCurrencyGainLogger::Init()
{
	const FString Dir = FPaths::ProjectSavedDir() / TEXT("Logs");
	IFileManager::Get().MakeDirectory(*Dir, /*Tree*/true);

	const FString Stamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
	FilePath = Dir / FString::Printf(TEXT("CurrencyGain_%s.csv"), *Stamp);

	// 헤더 작성
	AppendLine(TEXT("Timestamp,Type,Amount,Source,Context"));
}

void UCurrencyGainLogger::AppendLine(const FString& Line)
{
	FScopeLock Lock(&Mutex);
	FFileHelper::SaveStringToFile(Line + LINE_TERMINATOR, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

void UCurrencyGainLogger::LogGain(EElementalType Type, int32 Amount, const FName& Source, const FString& Context)
{
	// 콘솔/에디터 로그로도 확인하고 싶으면:
	UE_LOG(LogCurrency, Display, TEXT("Gain %d of %d (Source=%s) %s"),
		Amount, static_cast<int32>(Type), *Source.ToString(), *Context);

	const FString Now = FDateTime::Now().ToIso8601();
	const FString Line = FString::Printf(
		TEXT("%s,%d,%d,%s,%s"),
		*Now,
		static_cast<int32>(Type),
		Amount,
		*Source.ToString(),
		*Context.Replace(TEXT(","), TEXT(";")) // CSV 안전
	);
	AppendLine(Line);
}