// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SOWAbilitySystemComponent.h"

bool USOWAbilitySystemComponent::TryActivateAbilityWithTag(FGameplayTag InAbilityTag)
{
	checkf(InAbilityTag.IsValid(), TEXT("Ability Tag has not been assgined or the tag is invailid."));


	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(InAbilityTag.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty()) {
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		if (!SpecToActivate->IsActive()) {
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
}

void USOWAbilitySystemComponent::OnAbilityInputPressed(FGameplayTag InInputTag)
{
	// 키 바인딩에 따른 콜백 함수로써 동작합니다.
	// SOWCharacterPlayer에서 키 바인딩의 콜백 함수에서 해당 함수를 실행하고, 적절한 InputTag를 전달하면 매핑된 어빌리티가 실행됩니다.
	// 현재 일회성 능력만 처리할 수 있습니다.
	
	if (!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void USOWAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag InInputTag)
{
}

