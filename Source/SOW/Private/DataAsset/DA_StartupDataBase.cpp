// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_StartupDataBase.h"
#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "GameplayEffect.h"

void UDA_StartupDataBase::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	checkf(InASCToGive, TEXT("ASC was not assigned."));

	GrantAbility(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbility(ReactiveAbilities, InASCToGive, ApplyLevel);

	if (!StartupGameplayEffects.IsEmpty()) {
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : StartupGameplayEffects) {
			checkf(GameplayEffect, TEXT("GameplayEffect is invalid. Please Check Data Asset for effects"));

			UGameplayEffect* Effect = GameplayEffect->GetDefaultObject<UGameplayEffect>();

			InASCToGive->ApplyGameplayEffectToSelf(
				Effect, ApplyLevel, InASCToGive->MakeEffectContext()
			);
		}
	}
}

void UDA_StartupDataBase::GrantAbility(const TArray<TSubclassOf<USOWGameplayAbilityBase>>& InAbilitiesToGive, USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	checkf(InASCToGive, TEXT("ASC was not assigned."));

	for (const TSubclassOf<USOWGameplayAbilityBase>& AbilityToGive : InAbilitiesToGive) {
		checkf(AbilityToGive, TEXT("GameplayAbility is invalid. Please Check Data Asset for abilities."));

		FGameplayAbilitySpec AbilitySpec(AbilityToGive);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}

}
