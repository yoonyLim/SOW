// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SOWEnemyGameplayAbility.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"

ASOWCharacterEnemyBase* USOWEnemyGameplayAbility::GetSOWEnemyCharacterFromActorInfo() const
{
	return Cast<ASOWCharacterEnemyBase>(CurrentActorInfo->AvatarActor);
}
