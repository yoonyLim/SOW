// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SummonManager.h"

void USummonManager::Initialize()
{
	FGameplayTag NatureTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));
	FGameplayTag ElectroTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Electro"));
	FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Death"));
	FGameplayTag IceTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Ice"));
	FGameplayTag WaveTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Wave"));
	FGameplayTag DivinityTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Dvinity"));
	FGameplayTag MadnessTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Madness"));
	FGameplayTag FlameTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Flame"));

	M_CircleLevel.Add(NatureTag, 0);
	M_CircleLevel.Add(ElectroTag, 0);
	M_CircleLevel.Add(DeathTag, 0);
	M_CircleLevel.Add(IceTag, 0);
	M_CircleLevel.Add(WaveTag, 0);
	M_CircleLevel.Add(DivinityTag, 0);
	M_CircleLevel.Add(MadnessTag, 0);
	M_CircleLevel.Add(FlameTag, 0);
}

uint8 USummonManager::GetCircle(FGameplayTag Element)
{
	SummonStart.Broadcast();

	int32 TotalWeight = 0;

	if (M_CircleLevel[Element] == 0) return 0;

	for (int32 Weight : M_CircleProbability[M_CircleLevel[Element]])
	{
		TotalWeight += Weight;
	}

	if (TotalWeight <= 0)
	{
		return 0;
	}

	int32 RandomValue = FMath::RandRange(1, TotalWeight);
	int32 Accumulated = 0;

	for (int32 Index = 0; Index < M_CircleProbability[M_CircleLevel[Element]].Num(); ++Index)
	{
		Accumulated += M_CircleProbability[M_CircleLevel[Element]][Index];
		if (RandomValue <= Accumulated)
		{
			return Index + 1;
		}
	}

	return 0;
}

uint8 USummonManager::GetCircleLevel(FGameplayTag Element)
{
	return M_CircleLevel[Element];
}

void USummonManager::SetCircleLevel(FGameplayTag Element, uint8 CircleLevel)
{
	M_CircleLevel[Element] = CircleLevel;
}

TMap<uint8, TArray<uint8>>* USummonManager::GetSpellCompMap(EElementalType Element, uint8 Step)
{
	FString ElementName = StaticEnum<EElementalType>()->GetNameStringByValue(static_cast<int32>(Element));

	UE_LOG(LogTemp, Warning, TEXT("[Element: %s] [Step: %d]"), *ElementName, Step);

	switch (Element)
	{
	case EElementalType::Nature:
	{
		if (Step == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("hrerere"));

			return &M_NatureSpellComp_1;
		}
		else
		{
			return &M_NatureSpellComp_2;
		}
		break;
	}
	case EElementalType::Electro:
	{
		if (Step == 1)
		{
			return &M_ElectroSpellComp_1;
		}
		else
		{
			return &M_ElectroSpellComp_2;
		}
		break;
	}
	case EElementalType::Death:
	{
		if (Step == 1)
		{
			return &M_DeathSpellComp_1;
		}
		else
		{
			return &M_DeathSpellComp_2;
		}
		break;
	}
	case EElementalType::Ice:
	{
		if (Step == 1)
		{
			return &M_IceSpellComp_1;
		}
		else
		{
			return &M_IceSpellComp_2;
		}
		break;
	}
	case EElementalType::Wave:
	{
		if (Step == 1)
		{
			return &M_WaveSpellComp_1;
		}
		else
		{
			return &M_WaveSpellComp_2;
		}
		break;
	}
	case EElementalType::Divinity:
	{
		if (Step == 1)
		{
			return &M_DivinitySpellComp_1;
		}
		else
		{
			return &M_DivinitySpellComp_2;
		}
		break;
	}
	case EElementalType::Madness:
	{
		if (Step == 1)
		{
			return &M_MadnessSpellComp_1;
		}
		else
		{
			return &M_MadnessSpellComp_2;
		}
		break;
	}
	case EElementalType::Flame:
	{
		if (Step == 1)
		{
			return &M_FlameSpellComp_1;
		}
		else
		{
			return &M_FlameSpellComp_2;
		}
		break;
	}
	}
	return &M_FlameSpellComp_1;
}
