#include "Sound/SoundManager.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundActor.h"

USoundManager* USoundManager::Instance = nullptr;

USoundManager* USoundManager::Get(UObject* WorldContext)
{
	if (!Instance || !IsValid(Instance))
	{
		Instance = NewObject<USoundManager>();
		Instance->AddToRoot(); // GC 방지
	}

	if (WorldContext)
	{
		if (UWorld* World = WorldContext->GetWorld())
		{
			if (Instance->CachedWorld != World || !IsValid(Instance->SoundActor))
			{
				Instance->Init(World);
			}
		}
	}

	return Instance;
}

void USoundManager::Init(UWorld* World)
{
	if (!World) return;

	CachedWorld = World;
	SoundActor = nullptr;
	BGMComponent = nullptr;

	EnsureSoundActor();
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


void USoundManager::EnsureSoundActor()
{
	if (!CachedWorld) return;

	for (TActorIterator<ASoundActor> It(CachedWorld); It; ++It)
	{
		SoundActor = *It;
		break;
	}

	if (!IsValid(SoundActor))
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Name = TEXT("AudioManagerActor");

		SoundActor = CachedWorld->SpawnActor<ASoundActor>(
			ASoundActor::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);
	}

	if (IsValid(SoundActor))
	{
		BGMComponent = SoundActor->BGMComponent;
		if (BGMComponent)
		{
			BGMComponent->SetVolumeMultiplier(BGMVolume);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USoundManager: Failed to spawn/find AudioManagerActor"));
	}
}

