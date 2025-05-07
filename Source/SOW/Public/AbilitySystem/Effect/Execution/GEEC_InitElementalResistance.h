// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_InitElementalResistance.generated.h"

class UDataTable;
struct FElementResistanceData;
/**
 * 
 */
UCLASS()
class SOW_API UGEEC_InitElementalResistance : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

private:

	TArray<FGameplayTag> GetMatchedElementTags(const FGameplayTagContainer* SourceTags) const;
	FElementResistanceData* GetResistanceDataRow(const TArray<FGameplayTag>& MatchedElementTags) const;
public:

	UGEEC_InitElementalResistance();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	UPROPERTY()
	UDataTable* ResistanceData;
};
