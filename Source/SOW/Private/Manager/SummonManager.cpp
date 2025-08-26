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

static const TArray<int32> RarityWeights = { 83, 15, 2 }; // {common, rare, epic}

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
		TEXT("/Game/01Blueprints/DataTable/Turrets/DT_TurretSummonProb.DT_TurretSummonProb")
	);

	InitTurretArray();
}

void USummonManager::InitTurretArray()
{
	L_Common.Reset();
	L_Rare.Reset();
	L_Epic.Reset();

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
		}
	}
}

FSummonData USummonManager::RNG()
{
	int32 FullWeight = RarityWeights[0] + RarityWeights[1] + RarityWeights[2];

	const int32 RarityIdx = GachaRNG::DrawWeightedIndex(RarityWeights);

	const TArray<FSummonData>* Pool = nullptr;
	switch (RarityIdx)
	{
	case 0: Pool = &L_Common; break;
	case 1: Pool = &L_Rare; break;
	case 2: Pool = &L_Epic; break;
	}

	const uint32 idx = GachaRNG::UniformIndex((uint32)Pool->Num());
	return (*Pool)[(int32)idx];
}

bool USummonManager::TurretSummon()
{
	FSummonData TurretToSummon = RNG();

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

