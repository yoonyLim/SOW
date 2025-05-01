// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_StartupDataBase.h"
#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "GameplayEffect.h"

void UDA_StartupDataBase::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	GrantAbility(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbility(ReactiveAbilities, InASCToGive, ApplyLevel);
}

void UDA_StartupDataBase::GrantAbility(const TArray<TSubclassOf<USOWGameplayAbilityBase>>& InAbilitiesToGive, USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	for (const TSubclassOf<USOWGameplayAbilityBase>& AbilityToGive : InAbilitiesToGive) {
		if (!AbilityToGive) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityToGive);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}

}
