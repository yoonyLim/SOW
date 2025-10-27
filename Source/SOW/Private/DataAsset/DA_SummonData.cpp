// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_SummonData.h"

int8 UDA_SummonData::GetCostByRarity(ERarity Rarity)
{
	return CostByRank[Rarity];
}

FSummonProb UDA_SummonData::GetSummonProbByShopRank(int8 ShopRank)
{
	return SummonProbs[ShopRank];
}

int8 UDA_SummonData::GetProbByRarity(int8 ShopRank, ERarity Rarity)
{
	switch (Rarity)
	{
	case ERarity::Common:
		return SummonProbs[ShopRank].CommonProb;
	case ERarity::Rare:
		return SummonProbs[ShopRank].RareProb;
	case ERarity::Epic:
		return SummonProbs[ShopRank].EpicProb;
	case ERarity::Legendary:
		return SummonProbs[ShopRank].LegendaryProb;
	case ERarity::Origin:
		return SummonProbs[ShopRank].OriginProb;
	}
	return 0;
}


int8 UDA_SummonData::GetProbByShopRank(int8 ShopRank)
{
	return ShopRankUpProbs[ShopRank];
}

int8 UDA_SummonData::GetReloadCostByShopRank(int8 ShopRank)
{
	return ReloadCostByShopRank[ShopRank];
}