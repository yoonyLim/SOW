// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageLoggingManager.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UDamageLoggingManager : public UObject
{
	GENERATED_BODY()

public:
	// CSV ·Î±ë ÇÔ¼ö
	static void LogDamageToCSV(
		const FString& TurretInstanceID,
		const FString& TurretID,
		const FString& TargetID,
		float BaseDamage,
		float FinalDamage,
		float TargetRemainingHP,
		const FVector& TargetPos,
		int32 Stage = 0,
		AActor* AttackerActor = nullptr
	);

private:
	static FString GetActiveTagsAsString(AActor* Actor);

	static FString GetCurrentSessionID();
};
