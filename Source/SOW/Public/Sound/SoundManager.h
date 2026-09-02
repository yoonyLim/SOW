// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "SoundManager.generated.h"

class ASoundActor;

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
	UFUNCTION(BlueprintCallable)
	void PlayBGM(USoundBase* BGM, float FadeTime = 1.0f);

	UFUNCTION(BlueprintCallable)
	void StopBGM(float FadeTime = 1.0f);

	UFUNCTION(BlueprintCallable)
	void PlaySFX(USoundBase* SFX, FVector Location = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable)
	void SetBGMVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float Volume);

	void Init(UWorld* World);
private:
	// Singleton 인스턴스
	static USoundManager* Instance;

	// 내부 상태
	UPROPERTY()
	UAudioComponent* BGMComponent;

	UPROPERTY()
	ASoundActor* SoundActor = nullptr;

	UPROPERTY()
	UWorld* CachedWorld;

	float BGMVolume = 100.0f;
	float SFXVolume = 100.0f;

	void EnsureSoundActor();
};
