// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SummonManager.h"

#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"
#include "Utilities/GatchaRNG.h"
#include "Core/SOWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Tile/TileBase.h"
#include "Engine/World.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"

#include "AIController.h" // added by pgh
#include "GameModes/WaveGameMode.h"

bool FSummonData::operator==(const FSummonData& Other) const
{
	return TurretName == Other.TurretName
		&& Rarity == Other.Rarity
		&& TurretClass.Get() == Other.TurretClass.Get();
}

void USummonManager::Initialize()
{
	DT_SummonTurretProb = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/01Blueprints/DataTable/Turrets/DT_TurretSummonProb_Implemented.DT_TurretSummonProb_Implemented") // Testable Turret Only
	);

	InitTurretArray();
}

void USummonManager::InitTurretArray()
{
	L_Common.Reset();
	L_Rare.Reset();
	L_Epic.Reset();
	L_Legendary.Reset();
	L_Origin.Reset();

	if (!DT_SummonTurretProb)
	{
		UE_LOG(LogTemp, Warning, TEXT("SummonManager: DT_SummonTurretProb is null."));
		return;
	}

	for (const auto& Pair : DT_SummonTurretProb->GetRowMap())
	{
		const FName RowName = Pair.Key;
		const FSummonData* Row = reinterpret_cast<const FSummonData*>(Pair.Value);
		if(!Row) continue;

		switch (Row->Rarity)
		{
		case ERarity::Common:
			L_Common.AddUnique(*Row);
			break;
		case ERarity::Rare:
			L_Rare.AddUnique(*Row);
			break;
		case ERarity::Epic:
			L_Epic.AddUnique(*Row);
			break;
		case ERarity::Legendary:
			L_Legendary.AddUnique(*Row);
			break;
		case ERarity::Origin:
			L_Origin.AddUnique(*Row);
			break;
		}
	}
}

FSummonData USummonManager::RNG(TArray<int32> RarityWeights)
{
	const int32 RarityIdx = GachaRNG::DrawWeightedIndex(RarityWeights);

	const TArray<FSummonData>* Pool = nullptr;
	switch (RarityIdx)
	{
	case 0: Pool = &L_Common; break;
	case 1: Pool = &L_Rare; break;
	case 2: Pool = &L_Epic; break;
	case 3: Pool = &L_Legendary; break;
	case 4: Pool = &L_Origin; break;
	}

	const uint32 idx = GachaRNG::UniformIndex((uint32)Pool->Num());
	return (*Pool)[(int32)idx];
}

FSummonData USummonManager::RNGOriginTurret()
{
	const TArray<FSummonData>* Pool = &L_Origin;

	const uint32 idx = GachaRNG::UniformIndex((uint32)Pool->Num());
	return (*Pool)[(int32)idx];
}

bool USummonManager::TurretSummon(FSummonData TurretData)
{
	/*if (USOWBlueprintFunctionLibrary::GetCurrency(GetWorld()) < 10)
		return false;*/

	AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GM && !GM->SpawnTurret())
	{
		return false;
	}
	
	FSummonData TurretToSummon = TurretData;

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

			ASOWCharacterTurretBase* NewTurret = GetWorld()->SpawnActor<ASOWCharacterTurretBase>(TurretToSummon.TurretClass, SpawnLoc, SpawnRotator, Params);
		

			// added by pgh
			// possess AIController to Summoning Turret
			if (NewTurret)
			{
				// AIController�� �����ϰ� ����
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

			OnSummonTurret.Broadcast(TurretToSummon);

			return true;
		}
		else
		{
			continue;
		}
	}
	return false;
}

