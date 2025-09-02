// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityStstem/Ability/SOWCoreRuneGameplayAbility.h"

#include "Characters/CoreRune/SOWCharacterCoreRune.h"

ASOWCharacterCoreRune* USOWCoreRuneGameplayAbility::GetSOWCoreRuneCharacterFromActorInfo() const
{
	return Cast<ASOWCharacterCoreRune>(CurrentActorInfo->AvatarActor);
}
