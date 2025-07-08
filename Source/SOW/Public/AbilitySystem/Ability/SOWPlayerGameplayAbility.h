// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "SOWPlayerGameplayAbility.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class SOW_API USOWPlayerGameplayAbility : public USOWGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	/** Ability 가 발동 가능한지 상태 체크를 위한 함수, 마나 체크 */
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	/** 마나 소모 처리 함수, BP에서 사용 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Cost")
	void ApplyPlayerAbilityCost();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cost")
	float ManaCost = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cost")
	TSubclassOf<UGameplayEffect> ManaCostEffect;
};
