// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/GEC_SendGameplayEvent.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


void UGEC_SendGameplayEvent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{

	const FGameplayEffectContextHandle& EffectContext = GESpec.GetEffectContext();
	AActor* SourceActor = EffectContext.GetOriginalInstigator(); // ¶Ç´Â GetInstigator()

	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	AActor* TargetActor = TargetASC->GetAvatarActor();


	FGameplayEventData Data = MakeOugingGameplayEventData(ActiveGEContainer, GESpec, PredictionKey);

	for (const FGameplayTag& EventTagToSend : TagsToSend) {
		if (!EventTagToSend.IsValid()) continue;

		switch (TargetToSend)
		{
		case ESendTarget::Source:
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceActor, EventTagToSend, Data);
			break;
		case ESendTarget::Target:
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, EventTagToSend, Data);
			break;
		default:
			break;
		}
		

		//UE_LOG(LogTemp, Warning, TEXT("Send Event to %s"), *AppliedToASC.GetAvatarActor()->GetActorNameOrLabel());
	}
	
}

FGameplayEventData UGEC_SendGameplayEvent::MakeOugingGameplayEventData(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	FGameplayEventData Data;
	return Data;
}

