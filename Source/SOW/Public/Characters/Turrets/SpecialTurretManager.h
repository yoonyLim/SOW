// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"

#include "UObject/NoExportTypes.h"
#include "SpecialTurretManager.generated.h"

class ASOWCharacterTurretSpecialBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSynergyTurretDelegate, EGlacioStatType, StatType, float, value);
/**
 * 
 */
UCLASS()
class SOW_API USpecialTurretManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDelegate OnTurretSummoned;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDelegate OnTurretDead;

protected:
	TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioClass;

	ASOWCharacterTurretSpecialBase* SummonedGlacio;
};
