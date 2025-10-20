#include "Sound/SoundManager.h"
#include "Kismet/GameplayStatics.h"

USoundManager* USoundManager::Instance = nullptr;

USoundManager* USoundManager::Get(UObject* WorldContext)
{
	if (!Instance || !IsValid(Instance))
	{
		Instance = NewObject<USoundManager>();
		Instance->AddToRoot();
	}

	UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
	if (World)
	{
		// 매번 유효한 월드로 갱신
		if (Instance->CachedWorld != World)
		{
			Instance->CachedWorld = World;

			// 월드가 바뀌면 BGMComponent 재생성
			if (Instance->BGMComponent)
			{
				Instance->BGMComponent->DestroyComponent();
				Instance->BGMComponent = nullptr;
			}

			Instance->BGMComponent = NewObject<UAudioComponent>(World);
			if (Instance->BGMComponent)
			{
				Instance->BGMComponent->RegisterComponent();
				Instance->BGMComponent->bAutoActivate = false;
			}
		}
	}

	return Instance;
}
void USoundManager::Init(UWorld* World)
{
	CachedWorld = World;

	// BGM용 오디오 컴포넌트 초기화
	BGMComponent = NewObject<UAudioComponent>(World);
	BGMComponent->RegisterComponent();
	BGMComponent->bAutoActivate = false;
}

void USoundManager::PlayBGM(USoundBase* BGM, float FadeTime)
{
	if (!BGM || !BGMComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayBGM failed: Invalid BGM or BGMComponent"));
		return;
	}

	if (BGMComponent->IsPlaying())
		BGMComponent->FadeOut(FadeTime, 0.0f);

	BGMComponent->SetSound(BGM);
	BGMComponent->FadeIn(FadeTime, BGMVolume);
}

void USoundManager::StopBGM(float FadeTime)
{
	if (BGMComponent && BGMComponent->IsPlaying())
		BGMComponent->FadeOut(FadeTime, 0.0f);
}

void USoundManager::PlaySFX(USoundBase* SFX, FVector Location)
{
	if (!CachedWorld || !SFX) return;

	UGameplayStatics::PlaySoundAtLocation(CachedWorld, SFX, Location, SFXVolume);
}

void USoundManager::SetBGMVolume(float Volume)
{
	BGMVolume = Volume;
	if (BGMComponent)
		BGMComponent->SetVolumeMultiplier(Volume);
}

void USoundManager::SetSFXVolume(float Volume)
{
	SFXVolume = Volume;
}
