// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWStructTypes.h"
#include "SOWEnumTypes.h"
#include "SummonManager.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API USummonManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void SetCircleLevel(FGameplayTag Element, uint8 CircleLevel);

	UFUNCTION(BlueprintCallable)
	uint8 GetCircleLevel(FGameplayTag Element);

	UFUNCTION(BlueprintCallable)
	uint8 GetCircle(FGameplayTag Element);

	TMap<uint8, TArray<uint8>>* GetSpellCompMap(EElementalType Element, uint8 Step);


private:
	TMap<FGameplayTag, uint8> M_CircleLevel;

	TMap<uint8, TArray<uint8>> M_NatureSpellComp_1 = { {2, {2}}, {3, {3}}, {4, {5}}, {5, {2, 4, 5}} };
	TMap<uint8, TArray<uint8>> M_NatureSpellComp_2 = { {22, {1}}, {33, {3, 5}}, {45, {1}}, {52, {3}}, {54, {5}}, {55, {2, 4}} };

	TMap<uint8, TArray<uint8>> M_ElectroSpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_ElectroSpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_DeathSpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_DeathSpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_IceSpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_IceSpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_WaveSpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_WaveSpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_DivinitySpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_DivinitySpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_MadnessSpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_MadnessSpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_FlameSpellComp_1 = { };
	TMap<uint8, TArray<uint8>> M_FlameSpellComp_2 = { };

	TMap<uint8, TArray<uint8>> M_CircleProbability = { {1, {100, 0, 0, 0, 0}},
													   {2, {75, 25, 0, 0, 0}},
													   {3, {55, 30, 15, 0, 0}},
													   {4, {30, 40, 25, 5, 0}},
													   {5, {17, 24, 32, 24, 3}}
	};
};
