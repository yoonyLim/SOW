// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SOWCharacterUIComponent.h"
#include "SOWEnemyUIComponent.generated.h"

class ASOWCharacterEnemyBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTagChangedDelegate);
/**
 * 
 */
UCLASS()
class SOW_API USOWEnemyUIComponent : public USOWCharacterUIComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTagChangedDelegate OnTagChanged;

	UFUNCTION(BlueprintPure)
	ASOWCharacterEnemyBase* GetOwnerEnemy() const;
};
