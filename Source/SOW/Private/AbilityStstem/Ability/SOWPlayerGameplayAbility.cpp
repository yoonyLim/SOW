// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SOWPlayerGameplayAbility.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayAbilities/Public/GameplayEffect.h"


bool USOWPlayerGameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ManaCost > 0.0f)
	{
		const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		if (!ASC) return false;

		const USOWAttributeSet* AttributeSet = ASC->GetSet<USOWAttributeSet>();
		if (!AttributeSet) return false;


		const float CurrentMana = AttributeSet->GetCurrentMana();

		if (CurrentMana < ManaCost)
		{
			if (OptionalRelevantTags)
			{
				OptionalRelevantTags->AddTag(FGameplayTag::RequestGameplayTag("Player.Status.Fail.Ability"));
			}
			return false;
		}
	}

	return true;
}

void USOWPlayerGameplayAbility::ApplyPlayerAbilityCost()
{
	if (ManaCost <= 0.0f || !ManaCostEffect) return;

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ManaCostEffect, 1.0f);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag("Data.ManaCost"),
			(ManaCost * -1)
		);

		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
}