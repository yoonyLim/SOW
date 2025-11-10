// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SOWEnumTypes.h"
#include "SOWLogFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWave, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogDamage1, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTurret, Log, All);

/**
 * 
 */

UCLASS()
class SOW_API USOWLogFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 콘솔/에디터 로그
	void WaveInitialize();
	
	// ---- CSV: 타입드 위임 ----
	UFUNCTION(BlueprintCallable, Category="SOW|Log|CSV", meta=(WorldContext="WorldContextObject"))
	void LogWaveToCSV(
		EElementalType Type,
		int32 Amount,
		const FName& Source,
		const FString& Context
		);

	UFUNCTION(BlueprintCallable, Category="SOW|Log|CSV", meta=(WorldContext="WorldContextObject"))
	static void LogDamageToCSV(
		const FString& TurretInstanceID,
		const FString& TurretID,
		const FString& TargetID,
		float BaseDamage,
		float FinalDamage,
		float TargetRemainingHP,
		const FVector& TargetPos,
		int32 Stage = 0,
		AActor* AttackerActor = nullptr,
		AActor* DamagedActor = nullptr
		);

	UFUNCTION(BlueprintCallable, Category = "SOW|Log|CSV", meta = (WorldContext = "WorldContextObject"))
	static void LogCurrencyToCSV(
		const FString& CurrencyGenerator,
		int32 Stage = 0,
		int32 Currency = 0
	);


private:

	FCriticalSection Mutex;
	FString FilePath;
	void WaveAppendLine(const FString& Line);
	
	static FString GetActiveTagsAsString(AActor* Actor);

	//static FString GetDamagedTagsAsString(AActor* Actor);

	static FString GetCurrentSessionID();
};
