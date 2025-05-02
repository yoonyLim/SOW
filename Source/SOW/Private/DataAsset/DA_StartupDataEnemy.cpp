// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_StartupDataEnemy.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/Ability/SOWEnemyGameplayAbility.h"


void UDA_StartupDataEnemy::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const TSubclassOf<USOWEnemyGameplayAbility>& AbilityToGive : EnemyCombatAbilities) {
		if (!AbilityToGive) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityToGive);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}


}
