// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaveGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOneTimeCurrencyChanged, int32, TotalCurrency);

/**
 * 
 */
UCLASS()
class SOW_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

	int32 Currency = 0;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatcher")
	FOnOneTimeCurrencyChanged OnOneTimeCurrencyChanged;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Rune")
	void RuneUpdateHUD(float Health, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	void AddCurrency(int32 AddedCurrency);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	int32 GetCurrency() const { return Currency; };
};
