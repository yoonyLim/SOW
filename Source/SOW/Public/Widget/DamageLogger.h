// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageLogger.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SOW_API UDamageLogger : public UObject
{
	GENERATED_BODY()

	float BaseDamage;
	float FinalDamage;

public:
	UDamageLogger();
	UDamageLogger(float b, float f);

	UFUNCTION(BlueprintCallable)
	void SetLoggerValue(float b, float f);

	UFUNCTION(BlueprintCallable)
	void GetLoggerValue(float& OutBase, float& OutFinal) const;
	
};
