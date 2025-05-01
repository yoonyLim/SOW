// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DA_StartupDataBase.h"
#include "DA_StartupDataPlayer.generated.h"

class USOWPlayerGameplayAbility;
/**
 * 
 */
UCLASS()
class SOW_API UDA_StartupDataPlayer : public UDA_StartupDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<USOWPlayerGameplayAbility> > PlayerCombatAbilities;
};
