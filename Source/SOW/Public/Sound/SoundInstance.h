// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoundInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API USoundInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* StartBGM; 
};