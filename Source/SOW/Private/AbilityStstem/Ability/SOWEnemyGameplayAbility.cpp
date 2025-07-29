// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SOWEnemyGameplayAbility.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

ASOWCharacterEnemyBase* USOWEnemyGameplayAbility::GetSOWEnemyCharacterFromActorInfo() const
{
	return Cast<ASOWCharacterEnemyBase>(CurrentActorInfo->AvatarActor);
}

AEnemyBaseAIController* USOWEnemyGameplayAbility::GetSOWEnemyControllerFromActorInfo() const
{
	return Cast<AEnemyBaseAIController>(GetSOWEnemyCharacterFromActorInfo()->GetController());
}
