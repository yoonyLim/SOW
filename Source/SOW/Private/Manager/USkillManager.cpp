// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/USkillManager.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"

void UUSkillManager::Initialize(UDataTable* InSkillDataTable)
{
	SkillDataTable = InSkillDataTable;
	UnlockedSkillMap.Empty();
}

const FSkillData* UUSkillManager::FindSkillDataByID(const FName& SkillID) const
{
	if (!SkillDataTable) return nullptr;

	return SkillDataTable->FindRow<FSkillData>(SkillID, TEXT("Lookup SkillData"));
}

bool UUSkillManager::UnlockSkill(const FName& SkillID)
{
	if (IsSkillUnlocked(SkillID)) return false;

	const FSkillData* FoundData = FindSkillDataByID(SkillID);
	if (FoundData)
	{
		UnlockedSkillMap.Add(*FoundData);
		return true;
	}
	return false;
}

bool UUSkillManager::IsSkillUnlocked(const FName& SkillID) const
{
	return UnlockedSkillMap.ContainsByPredicate([&](const FSkillData& Skill)
		{
			return Skill.SkillID == SkillID;
		});
}

bool UUSkillManager::HasUnlockedSkillWithTag(FGameplayTag Tag) const
{
	return UnlockedSkillMap.ContainsByPredicate([&](const FSkillData& Skill)
		{
			return Skill.GrantTag == Tag;
		});
}

TArray<FSkillData> UUSkillManager::GetUnlockedSkillsByElement(FGameplayTag ElementTag) const
{
	TArray<FSkillData> Result;
	for (const FSkillData& Skill : UnlockedSkillMap)
	{
		if (Skill.SkillElementTag.MatchesTagExact(ElementTag))
		{
			Result.Add(Skill);
		}
	}
	return Result;
}

void UUSkillManager::GrantTagsToPlayer(AActor* Player, const TArray<FGameplayTag>& SelectedElements)
{
	if (!Player) return;

	UAbilitySystemComponent* ASC = Player->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return;

	for (const FSkillData& Skill : UnlockedSkillMap)
	{
		for (const FGameplayTag& Element : SelectedElements)
		{
			if (Skill.SkillElementTag.MatchesTagExact(Element))
			{
				if (Skill.GrantTag.IsValid())
				{
					ASC->AddLooseGameplayTag(Skill.GrantTag);
				}
			}
		}
	}
}