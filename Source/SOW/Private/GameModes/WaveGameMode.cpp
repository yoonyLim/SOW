// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WaveGameMode.h"

#include "SOWGameInstance.h"

void AWaveGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AWaveGameMode::AddCurrency(int32 AddedCurrency)
{
	Currency += AddedCurrency;
	OnOneTimeCurrencyChanged.Broadcast(Currency);
}
