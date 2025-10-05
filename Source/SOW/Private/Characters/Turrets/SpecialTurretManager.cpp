// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SpecialTurretManager.h"
#include "Characters/Turrets/SOWCharacterTurretSpecialBase.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "SOWGameplayTags.h"
#include "Tile/TileBase.h"
#include "AIController.h"
#include "Core/SOWPlayerController.h"

void USpecialTurretManager::Initialize(TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioTurret) {
   
    GlacioClass = GlacioTurret;
    
    AlphaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Alpha_I, 0);
    AlphaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Alpha_II, 0);
    AlphaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Alpha_III, 0);
    AlphaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Alpha_IV, 0);
    AlphaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Alpha_V, 0);

    BetaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Beta_I, 0);
    BetaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Beta_II, 0);
    BetaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Beta_III, 0);
    BetaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Beta_IV, 0);
    BetaPropertyCondition.Add(SOWGlacioPropertyTags::Property_Beta_V, 0);

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
    //if (!SummonedGlacio) return;

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

void USpecialTurretManager::RequestToApplyPropertyCondition(FGameplayTag ConditionTag)
{
    //if (!SummonedGlacio) return;

    if (AlphaPropertyCondition.Contains(ConditionTag)) {
        int newValue = AlphaPropertyCondition[ConditionTag] + 1;
        AlphaPropertyCondition.Add(ConditionTag, newValue);

        TArray<int> AllValues;
        AlphaPropertyCondition.GenerateValueArray(AllValues);
        for (int value : AllValues) {
            if (value == 0) {
                return;
            }
        }
        if (!SummonedGlacio) return;
        SummonedGlacio->OnAlphaConditionCheck.Broadcast(true);
    }
    else if (BetaPropertyCondition.Contains(ConditionTag)) {
        int newValue = BetaPropertyCondition[ConditionTag] + 1;
        BetaPropertyCondition.Add(ConditionTag, newValue);

        TArray<int> AllValues;
        BetaPropertyCondition.GenerateValueArray(AllValues);
        for (int value : AllValues) {
            if (value == 0) {
                return;
            }
        }
        if (!SummonedGlacio) return;
        SummonedGlacio->OnBetaConditionCheck.Broadcast(true);
    }

    else { return; }
}

void USpecialTurretManager::RequestToRemovePropertyCondition(FGameplayTag ConditionTag)
{
    //if (!SummonedGlacio) return;

    if (AlphaPropertyCondition.Contains(ConditionTag)) {
        int newValue = AlphaPropertyCondition[ConditionTag] - 1;
     
        if (SummonedGlacio && newValue == 0) SummonedGlacio->OnAlphaConditionCheck.Broadcast(false);
        AlphaPropertyCondition.Add(ConditionTag, newValue);
    }
    else if (BetaPropertyCondition.Contains(ConditionTag)) {
    
        int newValue = BetaPropertyCondition[ConditionTag] - 1;
       
        if (SummonedGlacio && newValue == 0) SummonedGlacio->OnBetaConditionCheck.Broadcast(false);
        BetaPropertyCondition.Add(ConditionTag, newValue);
    }

    else { return; }
}
