// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWStructTypes.h"
#include "SOWEnumTypes.h"
#include "Templates/SharedPointer.h"
#include "GameplayTagContainer.h"
#include "USkillManager.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTryUnlockSkill, )

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

	/* 특정 스킬이 해금 되었는지 확인하는 함수 */
	bool IsSkillUnlocked(const FName& SkillID) const;

	TArray<TSharedPtr<FSkillData>> GetUnlockedSkillsByElement(FGameplayTag ElementTag) const;

	UFUNCTION(BlueprintCallable)
	void ApplyPlayerSkillsToTurret(UAbilitySystemComponent* TargetASC, FGameplayTag ElementTag);

	UPROPERTY(BlueprintReadWrite)
	UDataTable* SkillDataTable;

	TArray<TSharedPtr<FSkillData>> UnlockedSkillList;

private:
	const FSkillData* FindSkillDataByID(const FName& SkillID) const;

	EElementalType StringToCurrencyType(const FString& InStr);
};

