// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GlobalCurrencyManager.h"

/* Need to fix when build game */
void UGlobalCurrencyManager::Initialize()
{
	for (int32 i = 0; i < static_cast<int32>(ECurrencyType::Max); i++)
	{
		ECurrencyType CurrencyType = static_cast<ECurrencyType>(i);
		if (!CurrentCurrency.Contains(CurrencyType))
		{
			/* Set All Currency value to 1000 for test */
			CurrentCurrency.Add(CurrencyType, 1000);
		}
	}
}

bool UGlobalCurrencyManager::AddCurrency(ECurrencyType CurrencyType, int32 Amount)
{
	if (CurrentCurrency.Contains(CurrencyType))
	{
		CurrentCurrency[CurrencyType] += Amount;
		OnCurrencyChanged.Broadcast(CurrentCurrency[CurrencyType], CurrencyType);
		return true;
	}
	else
	{
		return false;
	}
}

bool UGlobalCurrencyManager::SpentCurrency(ECurrencyType CurrencyType, int32 Amount)
{
	if (CurrentCurrency.Contains(CurrencyType))
	{
		if (CurrentCurrency[CurrencyType] < Amount)
		{
			return false;
		}
		else
		{
			CurrentCurrency[CurrencyType] -= Amount;
			OnCurrencyChanged.Broadcast(CurrentCurrency[CurrencyType], CurrencyType);
			return true;
		}
	}
	else
	{
		return false;
	}
}

