// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Manager/SummonManager.h"

#include "DA_SummonData.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UDA_SummonData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<int8, FSummonProb> SummonProbs;

	UPROPERTY(EditAnywhere)
	TMap<ERarity, int8> CostByRank;

	UPROPERTY(EditAnywhere)
	TMap<int8, int8> ShopRankUpProbs;

	UPROPERTY(EditAnywhere)
	TMap<int8, int8> ReloadCostByShopRank;

public:
	int8 GetCostByRarity(ERarity Rarity);

	FSummonProb GetSummonProbByShopRank(int8 ShopRank);

	int8 GetProbByRarity(int8 ShopRank, ERarity Rarity);

	int8 GetProbByShopRank(int8 ShopRank);

	int8 GetReloadCostByShopRank(int8 ShopRank);
};
