// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DA_StartupDataBase.h"
#include "DA_StartupDataTurret.generated.h"

class USOWTurretGameplayAbility;
/**
 * 
 */
UCLASS()
class SOW_API UDA_StartupDataTurret : public UDA_StartupDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<USOWTurretGameplayAbility> > TurretCombatAbilities;
public:
	virtual void GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
};
