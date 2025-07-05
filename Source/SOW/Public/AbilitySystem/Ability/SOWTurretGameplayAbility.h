// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "SOWTurretGameplayAbility.generated.h"

class ASOWCharacterTurretBase;
class USOWTurretCombatComponent;
/**
 * 
 */
UCLASS()
class SOW_API USOWTurretGameplayAbility : public USOWGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintPure)
	ASOWCharacterTurretBase* GetSOWTurretCharacterFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	USOWTurretCombatComponent* GetSOWTurretCombatComponentFromActorInfo() const;
};
