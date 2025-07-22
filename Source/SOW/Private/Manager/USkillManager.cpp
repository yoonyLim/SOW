// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/USkillManager.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SOWGameInstance.h"
#include "Manager/GlobalCurrencyManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h" 

void UUSkillManager::Initialize(UDataTable* InSkillDataTable)
{
	SkillDataTable = InSkillDataTable;
	UnlockedSkillList.Empty();
}

const FSkillData* UUSkillManager::FindSkillDataByID(const FName& SkillID) const
{
	if (!SkillDataTable) return nullptr;

	TArray<FName> RowNames = SkillDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		const FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(RowName, TEXT("FindSkillDataByID"));
		if (SkillData && SkillData->SkillID == SkillID)
		{
			return SkillData;
		}
	}
	return nullptr;
}

bool UUSkillManager::UnlockSkill(const FName& SkillID)
{
	if (IsSkillUnlocked(SkillID)) return false;

	const FSkillData* FoundData = FindSkillDataByID(SkillID);

	if (FoundData)
	{
		if (!IsSkillUnlocked(FoundData->RequiredSkill)) return false;

		USOWGameInstance* GI = Cast<USOWGameInstance>(UGameplayStatics::GetGameInstance(this));
		UGlobalCurrencyManager* GCM = GI->GetGlobalCurrencyManager();

		FString TagString = FoundData->SkillElementTag.ToString();
		FString LastStr;
		TagString.Split(TEXT("."), nullptr, &LastStr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		ECurrencyType CurrencyType = StringToCurrencyType(LastStr);

		if (GCM->GetCurrency(CurrencyType) >= FoundData->RequiredCurrencyAmount)
		{
			if (GCM->SpentCurrency(CurrencyType, FoundData->RequiredCurrencyAmount))
			{
				UE_LOG(LogTemp, Error, TEXT("Success Unlock Skill"));

				TSharedPtr<FSkillData> UnlockedSkill = MakeShared<FSkillData>(*FoundData);
				UnlockedSkillList.Add(UnlockedSkill);

				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Don't have enough currency"));
				return false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Don't have enough currency"));
			return false;
		}
	}
	return false;
}

bool UUSkillManager::IsSkillUnlocked(const FName& SkillID) const
{
	if (SkillID == "None")
	{
		return true;
	}
	return UnlockedSkillList.ContainsByPredicate([&](const TSharedPtr<FSkillData>& SkillPtr)
		{
			return SkillPtr->SkillID == SkillID;
		});
}

TArray<TSharedPtr<FSkillData>> UUSkillManager::GetUnlockedSkillsByElement(FGameplayTag ElementTag) const
{
	TArray<TSharedPtr<FSkillData>> Result;
	for (const TSharedPtr<FSkillData>& SkillPtr : UnlockedSkillList)
	{
		if (SkillPtr->SkillElementTag.MatchesTag(ElementTag))
		{
			Result.Add(SkillPtr);
		}
	}
	return Result;
}

void UUSkillManager::ApplyPlayerSkillsToTurret(UAbilitySystemComponent* TargetASC, FGameplayTag ElementTag)
{
	
}

ECurrencyType UUSkillManager::StringToCurrencyType(const FString& InStr)
{
	if (InStr.Equals("Nature", ESearchCase::IgnoreCase)) return ECurrencyType::Nature;
	if (InStr.Equals("Normal", ESearchCase::IgnoreCase)) return ECurrencyType::Normal;
	if (InStr.Equals("Elector", ESearchCase::IgnoreCase)) return ECurrencyType::Elector;
	if (InStr.Equals("Ice", ESearchCase::IgnoreCase)) return ECurrencyType::Ice;
	if (InStr.Equals("Death", ESearchCase::IgnoreCase)) return ECurrencyType::Death;
	if (InStr.Equals("Wave", ESearchCase::IgnoreCase)) return ECurrencyType::Wave;
	if (InStr.Equals("Divinity", ESearchCase::IgnoreCase)) return ECurrencyType::Divinity;
	if (InStr.Equals("Flame", ESearchCase::IgnoreCase)) return ECurrencyType::Flame;
	if (InStr.Equals("Madness", ESearchCase::IgnoreCase)) return ECurrencyType::Madness;

	return ECurrencyType::Max;
}
