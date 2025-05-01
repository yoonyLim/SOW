// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_StartupDataPlayer.h"

void UDA_StartupDataPlayer::GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	// Need to match Player Ability with Input Tags
}
