// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_InitTurretAttribute.generated.h"

class UDataTable;
struct FTurretAttributeData;
/**
 * 
 */
UCLASS()
class SOW_API UGEEC_InitTurretAttribute : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

private:

	
	//FTurretAttributeData* GetResistanceDataRow(const TArray<FGameplayTag>& MatchedElementTags) const;

public:

	UGEEC_InitTurretAttribute();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	UPROPERTY()
	UDataTable* AttributeData;
};
