// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"
#include "SOWGameplayTags.h"
#include "SOWTurret_TagPresent.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API USOWTurret_TagPresent : public USOWTurretGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Buff")
	FGameplayTag TagToPresent = SOWGameplayTags::Turret_Status_Buff_GlassyAura;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
