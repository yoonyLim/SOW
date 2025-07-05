// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_TurretEvolutionData.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Components/SOWTurretCombatComponent.h"

void UDA_TurretEvolutionData::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const TSubclassOf<USOWTurretGameplayAbility>& AbilityToGive : NewTurretCombatAbilities) {
		checkf(AbilityToGive, TEXT("GameplayAbility is invalid. Please Check Data Asset for abilities."));

		FGameplayAbilitySpec AbilitySpec(AbilityToGive);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		ClearOldCombatAbility(InASCToGive);

		InASCToGive->GiveAbility(AbilitySpec);
	}
}

void UDA_TurretEvolutionData::ClearOldCombatAbility(USOWAbilitySystemComponent* InASCToGive)
{

	TArray<FGameplayAbilitySpec> AbilitiesToRemove;
	ASOWCharacterTurretBase* Turret = Cast<ASOWCharacterTurretBase>(InASCToGive->GetAvatarActor());
	if (!Turret) { return; }

	USOWTurretCombatComponent* CombatComp = Turret->GetTurretCombatComponent();
	if (!CombatComp) { return; }



	for (const FGameplayAbilitySpec& Spec : InASCToGive->GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(CombatComp->GetAbilityTagToActivation()))
		{
			AbilitiesToRemove.Add(Spec);
		}
	}

	for (const FGameplayAbilitySpec& Spec : AbilitiesToRemove)
	{
		InASCToGive->ClearAbility(Spec.Handle);
	}
}
