// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_StartupDataPlayer.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/Ability/SOWPlayerGameplayAbility.h"

void UDA_StartupDataPlayer::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!PlayerCombatAbilities.IsEmpty()) {
		for (const FPlayerAbilitySet& AbilitySet : PlayerCombatAbilities) {
			
			FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
