// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SOWGameInstance.generated.h"

class UUSkillManager;
class UGlobalCurrencyManager;
class UOneTimeCurrencyManager;
class USummonManager;
class UDataTable;

/**
 * 
 */
UCLASS()
class SOW_API USOWGameInstance : public UGameInstance
{
    GENERATED_BODY()
    
public:

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UUSkillManager> SkillManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UGlobalCurrencyManager> GlobalCurrencyManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UOneTimeCurrencyManager> OneTimeCurrencyManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<USummonManager> SummonManager;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    TObjectPtr<UDataTable> SkillDataTable;

public:
    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    UUSkillManager* GetSkillManager() { return SkillManager; }

    UFUNCTION(BlueprintCallable)
    USummonManager* GetSummonManager() { return SummonManager; }

    UFUNCTION(BlueprintCallable)
    UGlobalCurrencyManager* GetGlobalCurrencyManager() { return GlobalCurrencyManager; }

    UFUNCTION(BlueprintCallable)
    UOneTimeCurrencyManager* GetOneTimeCurrencyManager() { return OneTimeCurrencyManager; }
};
