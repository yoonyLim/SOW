// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"

ASOWCharacterTurretBase* USOWTurretGameplayAbility::GetSOWTurretCharacterFromActorInfo() const
{
	return Cast<ASOWCharacterTurretBase>(CurrentActorInfo->AvatarActor);
}

USOWTurretCombatComponent* USOWTurretGameplayAbility::GetSOWTurretCombatComponentFromActorInfo() const
{
	return GetSOWTurretCharacterFromActorInfo()->GetTurretCombatComponent();
}
