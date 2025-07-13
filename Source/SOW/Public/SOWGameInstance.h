// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SOWGameInstance.generated.h"

class UUSkillManager;
class UDataTable;

/**
 * 
 */
UCLASS()
class SOW_API USOWGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    virtual void Init() override;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UUSkillManager> SkillManager;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    TObjectPtr<UDataTable> SkillDataTable;
};
