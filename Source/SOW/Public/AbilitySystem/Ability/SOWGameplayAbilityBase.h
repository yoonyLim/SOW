// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SOWGameplayAbilityBase.generated.h"

class USOWAbilitySystemComponent;
class ASOWCharacter;

UENUM(BlueprintType)
enum class ESOWAbilityActivationPolicy : uint8 {
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class SOW_API USOWGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure)
	USOWAbilitySystemComponent* GetSOWAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	ASOWCharacter* GetSOWPawnCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "SOWAbility")
	FActiveGameplayEffectHandle ApplyGameplayEffectToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle InSpecHandle);

	UPROPERTY(EditDefaultsOnly, Category = "SOWAbility")
	ESOWAbilityActivationPolicy AbilityActivationPolicy = ESOWAbilityActivationPolicy::OnTriggered;
};
