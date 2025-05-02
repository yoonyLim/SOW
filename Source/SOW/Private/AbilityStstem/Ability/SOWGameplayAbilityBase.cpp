// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SOWGameplayAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "Characters/SOWCharacter.h"

void USOWGameplayAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	/* 능력이 부여될 경우 실행되는 이벤트 함수 */
	// On Given 어빌리티가 부여되는 경우 즉시 실행

	Super::OnGiveAbility(ActorInfo, Spec);
	if (AbilityActivationPolicy == ESOWAbilityActivationPolicy::OnGiven) {
		if (ActorInfo && !Spec.IsActive()) {
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void USOWGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	/* 능력이 종료될 경우 실행되는 이벤트 함수 */
	// On Given 어빌리티가 종료되는 경우 즉시 제거

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (AbilityActivationPolicy == ESOWAbilityActivationPolicy::OnGiven) {
		if (ActorInfo) {
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}

}

USOWAbilitySystemComponent* USOWGameplayAbilityBase::GetSOWAbilitySystemComponentFromActorInfo() const
{
	return Cast<USOWAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

ASOWCharacter* USOWGameplayAbilityBase::GetSOWPawnCharacterFromActorInfo() const
{
	return Cast<ASOWCharacter>(CurrentActorInfo->AvatarActor);
}

FActiveGameplayEffectHandle USOWGameplayAbilityBase::ApplyGameplayEffectToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetSOWAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);
}
