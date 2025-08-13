// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/OneTimeCurrencyManager.h"

void UOneTimeCurrencyManager::Initialize()
{
	for (int32 i = 0; i < static_cast<int32>(EElementalType::Max); i++)
	{
		EElementalType CurrencyType = static_cast<EElementalType>(i);
		if (!CurrentCurrency.Contains(CurrencyType))
		{
			CurrentCurrency.Add(CurrencyType, 0);
		}
	}
}

bool UOneTimeCurrencyManager::AddCurrency(EElementalType CurrencyType, int32 Amount)
{
	/*
		CurrencyType:
		EElementalType::Nature,
		EElementalType::Electro,
		EElementalType::Death,
		EElementalType::Ice,
		EElementalType::Wave,
		EElementalType::Divinity,
		EElementalType::Madness,
		EElementalType::Flame,
	*/

	if (CurrentCurrency.Contains(CurrencyType))
	{
		CurrentCurrency[CurrencyType] += Amount;
		OnOneTimeCurrencyChanged.Broadcast(CurrentCurrency[CurrencyType], CurrencyType);
		return true;
	}
	else
	{
		return false;
	}
}

bool UOneTimeCurrencyManager::SpentCurrency(EElementalType CurrencyType, int32 Amount)
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
			OnOneTimeCurrencyChanged.Broadcast(CurrentCurrency[CurrencyType], CurrencyType);
			return true;
		}
	}
	else
	{
		return false;
	}
}
