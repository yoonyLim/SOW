// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "SOWEnemyGameplayAbility.generated.h"

class ASOWCharacterEnemyBase;
/**
 * 
 */
UCLASS()
class SOW_API USOWEnemyGameplayAbility : public USOWGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	ASOWCharacterEnemyBase* GetSOWEnemyCharacterFromActorInfo() const;
};
