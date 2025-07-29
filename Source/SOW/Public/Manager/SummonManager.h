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

	TMap<uint8, TArray<uint8>> M_NatureSpellComp_1 = { {1, {1}}, {2, {2}}, {3, {4}}, {4, {1, 2, 4}} };
	TMap<uint8, TArray<uint8>> M_NatureSpellComp_2 = { {11, {0}}, {22, {2, 4}}, {34, {0}}, {41, {2}}, {43, {4}}, {44, {1, 3}} };

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
