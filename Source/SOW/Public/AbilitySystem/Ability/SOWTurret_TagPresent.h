// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"
#include "SOWTurret_TagPresent.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API USOWTurret_TagPresent : public USOWTurretGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
