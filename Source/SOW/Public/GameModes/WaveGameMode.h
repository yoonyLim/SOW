// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SOWLogFunctionLibrary.h"
#include "WaveGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOneTimeCurrencyChanged, int32, TotalCurrency);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurretsNumChanged, int32, TotalTurrets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageEnd);

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
	int32 SummonedTurretsNum = 0;

	// 통화 획득 로거 인스턴스
	UPROPERTY() // GC 보호
	TObjectPtr<USOWLogFunctionLibrary> CurrencyLogger;

public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatcher")
	FOnOneTimeCurrencyChanged OnOneTimeCurrencyChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatcher")
	FOnTurretsNumChanged OnTurretsNumChanged;
	
	// fucking shit clear
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatcher")
	FOnStageEnd OnStageEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turerts")
	int32 MaxturretsNum = 10;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Rune")
	void RuneUpdateHUD(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Rune")
	void RuneUpdateShieldHUD(float Shield);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Rune")
	void RuneDestroyed();

	UFUNCTION(BlueprintCallable, Category = "Currency")
	void AddCurrency(int32 AddedCurrency);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	void AddCurrencyByTurret(int32 AddedCurrency, FString TurretName);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	int32 GetCurrency() const { return Currency; };

	UFUNCTION(BlueprintCallable, Category = "Turrets")
	int32 GetSummonedTurretsNum() const { return SummonedTurretsNum; }

	UFUNCTION(BlueprintCallable, Category = "Turrets")
	void AddSummonedTurretsNum() { SummonedTurretsNum++; }

	UFUNCTION(BlueprintCallable, Category = "Turrets")
	bool CanSpawnTurret();

	UFUNCTION(BlueprintCallable, Category = "Turrets")
	void SellTurret(int32 RefundCurrency);

	UFUNCTION(BlueprintCallable, Category = "Turrets")
	int32 CheckSpawnedTurretNum();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Alert")
	void AlertNotEnoughCurrency();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Alert")
	void AlertMaxTurrets();
};
