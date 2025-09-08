// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/OneTimeCurrencyManager.h"
#include "Log/CurrencyGainLogger.h"
#include "Log/CurrencyLog.h"

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

	// 로거 준비
	if (!GainLogger)
	{
		GainLogger = NewObject<UCurrencyGainLogger>(this);
		GainLogger->Init();
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

		if (GainLogger && Amount > 0)
		{
			GainLogger->LogGain(CurrencyType, Amount, /*Source*/NAME_None, /*Context*/TEXT(""));
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool UOneTimeCurrencyManager::AddCurrencyWithSource(EElementalType CurrencyType, int32 Amount, FName SourceTag, const FString& Context)
{
	const bool bOk = AddCurrency(CurrencyType, Amount);
	if (bOk && GainLogger && Amount > 0)
	{
		GainLogger->LogGain(CurrencyType, Amount, SourceTag, Context);
	}
	return bOk;
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
