// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWEnemyGameplayAbility.h"
#include "SOWGameplayTags.h"
#include "SOWEnemy_TagPresent.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API USOWEnemy_TagPresent : public USOWEnemyGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Dead")
	FGameplayTag TagToPresent = SOWGameplayTags::Shared_Status_Dead;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
