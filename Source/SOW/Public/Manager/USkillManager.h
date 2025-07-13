// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWStructTypes.h"
#include "SOWEnumTypes.h"
#include "GameplayTagContainer.h"
#include "USkillManager.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UUSkillManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UDataTable* InSkillDataTable);

	bool UnlockSkill(const FName& SkillID);

	bool IsSkillUnlocked(const FName& SkillID) const;

	TArray<FSkillData> GetUnlockedSkillsByElement(FGameplayTag ElementTag) const;

	/* 입력 Tag의 스킬이 해금 되었는지 확인하는 함수 */
	bool HasUnlockedSkillWithTag(FGameplayTag Tag) const;

	/** 게임 시작 시 플레이어에게 GrantTag 일괄 부여 */
	void GrantTagsToPlayer(AActor* Player, const TArray<FGameplayTag>& SelectedElements);


	UPROPERTY(BlueprintReadWrite)
	UDataTable* SkillDataTable;

	UPROPERTY(BlueprintReadWrite)
	TArray<FSkillData> UnlockedSkillMap;

private:
	const FSkillData* FindSkillDataByID(const FName& SkillID) const;
};
