// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SpecialTurretManager.h"
#include "Characters/Turrets/SOWCharacterTurretSpecialBase.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "SOWGameplayTags.h"
#include "Tile/TileBase.h"
#include "AIController.h"
#include "Core/SOWPlayerController.h"

void USpecialTurretManager::Initialize() {
    FStringClassReference GlacioBPRef(TEXT("/Game/01Blueprints/Turret/03Ice/00Glacio/BP_Turret_Special_Glacio.BP_Turret_Special_Glacio_C"));
    if (UClass* LoadedClass = GlacioBPRef.TryLoadClass<ASOWCharacterTurretSpecialBase>())
    {
        GlacioClass = LoadedClass;
    }

    OnSynergyChanged.AddDynamic(this, &USpecialTurretManager::ProcessGlacio);
}

void USpecialTurretManager::SummonGlacio()
{
    if (SummonedGlacio) return;
    // 이미 글라시오가 소환되어 있다면 무시합니다.

    ASOWPlayerController* SOWPC = Cast<ASOWPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    SOWPC->GetTileMap();

    for (AActor* CurrentTile : SOWPC->GetTileMap())
    {
        ATileBase* CT = Cast<ATileBase>(CurrentTile);

        if (CT->TileState == ETileSummonState::Available)
        {
            FVector SpawnLoc = CT->GetActorLocation();
            SpawnLoc.Z += 65.f;

            FRotator SpawnRotator(0.f, 180.f, 0.f);

            FActorSpawnParameters Params;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            Params.Owner = SOWPC;
            Params.Instigator = SOWPC->GetPawn();

            SummonedGlacio = GetWorld()->SpawnActor<ASOWCharacterTurretSpecialBase>(GlacioClass, SpawnLoc, SpawnRotator, Params);


            // added by pgh
            // possess AIController to Summoning Turret
            if (SummonedGlacio)
            {
                // AIController를 스폰하고 빙의
                if (!SummonedGlacio->GetController())
                {
                    AAIController* AIController = GetWorld()->SpawnActor<AAIController>(
                        AAIController::StaticClass(),
                        SpawnLoc,
                        SpawnRotator
                    );

                    if (AIController)
                    {
                        AIController->Possess(SummonedGlacio);
                    }
                }
            }
            // end possessing code

            CT->TileState = ETileSummonState::Occupied;
            return;
        }
    }
}

void USpecialTurretManager::RemoveGlacio()
{
	if (!SummonedGlacio) return;

	FGameplayTagContainer Tags;
	Tags.AddTag(SOWGameplayTags::Shared_Status_Dead);
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(SummonedGlacio, Tags);

	SummonedGlacio = nullptr;
}

ASOWCharacterTurretBase* USpecialTurretManager::GetGlacio()
{
    return Cast<ASOWCharacterTurretBase>(SummonedGlacio);
}

void USpecialTurretManager::ProcessGlacio(int SynergyCount)
{
	if (SynergyCount == 2) {
        UE_LOG(LogTemp, Warning, TEXT("Synergy 2. Try to Spawn Glacio"));
		SummonGlacio();
	}
	else if (SynergyCount < 2) {
		RemoveGlacio();
	}
	else {
		// ??
	}
}
