// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "SoundManager.generated.h"
/**
 * 
 */
UCLASS()
class SOW_API USoundManager : public UObject
{
	GENERATED_BODY()
public:
	static USoundManager* Get(UObject* WorldContext);

	// 사운드 관련 기능
	void PlayBGM(USoundBase* BGM, float FadeTime = 1.0f);
	void StopBGM(float FadeTime = 1.0f);
	void PlaySFX(USoundBase* SFX, FVector Location = FVector::ZeroVector);
	void SetBGMVolume(float Volume);
	void SetSFXVolume(float Volume);

private:
	// Singleton 인스턴스
	static USoundManager* Instance;

	// 내부 상태
	UPROPERTY()
	UAudioComponent* BGMComponent;

	UPROPERTY()
	UWorld* CachedWorld;

	float BGMVolume = 1.0f;
	float SFXVolume = 1.0f;

	void Init(UWorld* World);
};
