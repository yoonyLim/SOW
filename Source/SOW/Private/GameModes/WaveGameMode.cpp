// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WaveGameMode.h"
#include "Log/CurrencyGainLogger.h" // 로거 연결
#include "SOWGameInstance.h" // EElementalType 사용 

void AWaveGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 로거 생성 및 초기화
	CurrencyLogger = NewObject<UCurrencyGainLogger>(this);
	if (CurrencyLogger)
	{
		CurrencyLogger->Init();
	}
}

void AWaveGameMode::AddCurrency(int32 AddedCurrency)
{
	const int32 OldCurrency = Currency;
	Currency += AddedCurrency;

	// 기존 브로드캐스트 유지
	OnOneTimeCurrencyChanged.Broadcast(Currency);

	// 로깅 (타입이 명확하지 않으므로 None/Unknown 등 중립 타입 사용)
	/*if (CurrencyLogger)
	{
		const FString Context = FString::Printf(
			TEXT("Old=%d, Added=%d, Total=%d"),
			OldCurrency, AddedCurrency, Currency);

		// 프로젝트의 enum에 None이 없으면 적절한 기본값으로 교체하세요.
		CurrencyLogger->LogGain(EElementalType::Nature, AddedCurrency,
			FName(TEXT("WaveGameMode.AddCurrency")),
			Context);
	}*/
}