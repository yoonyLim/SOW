// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWEnemyGameplayAbility.h"
#include "GA_Enemy_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UGA_Enemy_RangedAttack : public USOWEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	/** Gameplay Event 수신 시 투사체 발사 */
	UFUNCTION()
	void OnAttackEventReceived(FGameplayEventData Payload);
};