// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void CreateWaveSystemHUD();
	

protected:
	virtual void BeginPlay() override;
};
