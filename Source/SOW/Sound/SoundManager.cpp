#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

USoundManager* USoundManager::Instance = nullptr;

USoundManager* USoundManager::Get(UObject* WorldContext)
{
	if (!Instance)
	{
		Instance = NewObject<USoundManager>();
		Instance->Init(WorldContext->GetWorld());
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
	if (!BGM || !BGMComponent) return;

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
