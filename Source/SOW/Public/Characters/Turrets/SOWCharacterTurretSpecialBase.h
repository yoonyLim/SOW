// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "SOWEnumTypes.h"
#include "SOWCharacterTurretSpecialBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSynergyTurretSummonDelegate, EGlacioStatType, StatType, float, value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSynergyTurretDeathDelegate, EGlacioStatType, StatType, float, value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPropertyConditionChecked, bool, HasChecked);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTurretSacrificedStatusAnnounced, ASOWCharacterTurretBase*, InTurret, float, NewAttack);
//
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurretSacrificedDeadAnnounced, ASOWCharacterTurretBase*, InTurret);

/**
 * 
 */
UCLASS()
class SOW_API ASOWCharacterTurretSpecialBase : public ASOWCharacterTurretBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretSummonDelegate OnSynergyAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDeathDelegate OnSynergyRemoved;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPropertyConditionChecked OnAlphaConditionCheck;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPropertyConditionChecked OnBetaConditionCheck;

	/*UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTurretSacrificedStatusAnnounced OnTurretSacrificedStatusAnnounced;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTurretSacrificedDeadAnnounced OnTurretSacrificedDeadAnnounced;*/
	
};
