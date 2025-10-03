// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SOWTurret_TagPresent.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWGameplayTags.h"

void USOWTurret_TagPresent::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

   // ActorInfo->AbilitySystemComponent
    if (ActorInfo && !Spec.IsActive() && ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(SOWGameplayTags::Turret_Status_Buff_GlassyAura)) {
        ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
    }
}
