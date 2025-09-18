// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWGameInstance.h"
#include "Engine/DataTable.h"
#include "Manager/USkillManager.h"
#include "Manager/GlobalCurrencyManager.h"
#include "Manager/OneTimeCurrencyManager.h"
#include "Manager/SummonManager.h"
#include "UObject/ConstructorHelpers.h"
#include "GameModes/WaveGameMode.h"

#include "SOWStructTypes.h"
#include "Core/SOWPlayerController.h"
#include "AIController.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Tile/TileBase.h"
#include "Kismet/GameplayStatics.h"

#include "SOWBlueprintFunctionLibrary.h"


void USOWGameInstance::Init()
{
	Super::Init();

    SkillManager = NewObject<UUSkillManager>(this);

    if (SkillManager && SkillDataTable)
    {
        SkillManager->Initialize(SkillDataTable);
    }

    GlobalCurrencyManager = NewObject<UGlobalCurrencyManager>(this);

    if (GlobalCurrencyManager)
    {
        GlobalCurrencyManager->Initialize();
    }

    OneTimeCurrencyManager = NewObject<UOneTimeCurrencyManager>(this);

    if (GlobalCurrencyManager)
    {
        OneTimeCurrencyManager->Initialize();
    }

    SummonManager = NewObject<USummonManager>(this);

    if (SummonManager)
    {
        SummonManager->Initialize();
    }
}

void USOWGameInstance::SummonTurret(FName TurretType)
{
    UDataTable* TurretTable = LoadObject<UDataTable>(
        nullptr,
        TEXT("/Game/01Blueprints/DataTable/Turrets/DT_TurretSummonProb.DT_TurretSummonProb")
    );

    if (!TurretTable) return;

    FSummonData* TurretToSummon = TurretTable->FindRow<FSummonData>(TurretType, TEXT(""));


    if (!TurretToSummon) return;


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

            ASOWCharacterTurretBase* NewTurret = GetWorld()->SpawnActor<ASOWCharacterTurretBase>(TurretToSummon->TurretClass, SpawnLoc, SpawnRotator, Params);


            // added by pgh
            // possess AIController to Summoning Turret
            if (NewTurret)
            {
                // AIController를 스폰하고 빙의
                if (!NewTurret->GetController())
                {
                    AAIController* AIController = GetWorld()->SpawnActor<AAIController>(
                        AAIController::StaticClass(),
                        SpawnLoc,
                        SpawnRotator
                    );

                    if (AIController)
                    {
                        AIController->Possess(NewTurret);
                    }
                }
            }
            // end possessing code

            CT->TileState = ETileSummonState::Occupied;
            return;
            //OnSummonTurret.Broadcast(TurretToSummon);
            // No Any Broadcast. this is just for debugging with random spawn
        }
        else
        {
            continue;
        }
    }
}

void USOWGameInstance::GetResource(float Count)
{
   // USOWBlueprintFunctionLibrary::RequestToGenerateOnTimeCurrency(GetWorld(), FGameplayTag::RequestGameplayTag("Shared.Element.Nature"), Count);
    AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(this));

    GM->AddCurrency(Count);
}
