// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DA_StartupDataBase.h"
#include "DA_TurretEvolutionData.generated.h"

class USOWTurretGameplayAbility;
/**
 * 
 */
UCLASS()
class SOW_API UDA_TurretEvolutionData : public UDA_StartupDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<USOWTurretGameplayAbility> > NewTurretCombatAbilities;
public:
	virtual void GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	void ClearOldCombatAbility(USOWAbilitySystemComponent* InASCToGive);
};
