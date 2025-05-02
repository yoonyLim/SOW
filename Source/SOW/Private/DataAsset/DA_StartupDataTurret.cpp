// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_StartupDataTurret.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"

void UDA_StartupDataTurret::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const TSubclassOf<USOWTurretGameplayAbility>& AbilityToGive : TurretCombatAbilities) {
		if (!AbilityToGive) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityToGive);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}
}
