// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SOWLogFunctionLibrary.h"
#include "WaveGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOneTimeCurrencyChanged, int32, TotalCurrency);

// 전방 선언: 로거는 헤더 의존 최소화
class UWaveLoggingManager;

/**
 * 
 */
UCLASS()
class SOW_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	// 재화 합계
	int32 Currency = 0;

	// 통화 획득 로거 인스턴스
	UPROPERTY() // GC 보호
	TObjectPtr<USOWLogFunctionLibrary> CurrencyLogger;

public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatcher")
	FOnOneTimeCurrencyChanged OnOneTimeCurrencyChanged;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Rune")
	void RuneUpdateHUD(float Health, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	void AddCurrency(int32 AddedCurrency);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	int32 GetCurrency() const { return Currency; };
};
