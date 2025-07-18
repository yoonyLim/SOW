// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/ShadowTileGimmickManager.h"
#include "Tile/SOWTileSpawnerActor.h"
#include "Tile/SOWTilePlacementHelper.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"


void AShadowTileGimmickManager::BeginPlay()
{
	Super::BeginPlay();
	TileSpawnerRef = Cast<ATileSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileSpawner::StaticClass()));
// 10초 간격으로 변경 수행
	GetWorld() -> GetTimerManager().SetTimer(TransformTimerHandle, this, &AShadowTileGimmickManager::PickRandomTileAndTransform, 10.0f, true, 5.0f);
}

void AShadowTileGimmickManager::PickRandomTileAndTransform()
{
	const int32 Width = TileSpawnerRef -> GridWidth;
	const int32 Height = TileSpawnerRef -> GridHeight;
	const auto& TileClasses = TileSpawnerRef -> GridTiles;
	auto& Spawned = TileSpawnerRef -> SpawnedTileActors;

	TArray<FIntPoint> validCoords;

	for (int32 X = 0; X < Width - 1; ++X)
	{
		for (int32 Y = 0; Y < Height - 1; ++Y)
		{
			int32 TileA = Y * Width + X;
			int32 TileB = Y * Width + X + 1;
			int32 TileC = (Y + 1) * Width + X;
			int32 TileD = (Y + 1) * Width + X + 1;

			if (TileClasses.IsValidIndex(TileD) && TileClasses[TileA] && TileClasses[TileB] && TileClasses[TileC] && TileClasses[TileD] &&
				Spawned.IsValidIndex(TileD) && Spawned[TileA] && Spawned[TileB] && Spawned[TileC] && Spawned[TileD])
			{
				validCoords.Add(FIntPoint(X, Y));
			}
		}
	}

	if (validCoords.Num() == 0) return;

	FIntPoint Start = validCoords[FMath::RandRange(0, validCoords.Num() - 1)];
	TArray<FIntPoint> Targets = {
		Start,
		{Start.X + 1, Start.Y},
		{Start.X, Start.Y + 1},
		{Start.X + 1, Start.Y + 1}
	};
	TArray<FRevertTileData> RevertTileDataList;

	for (const FIntPoint& Coord : Targets)
	{
		const int32 Index = Coord.Y * Width + Coord.X;

		AActor* OldTile = Spawned[Index];
		TSubclassOf<AActor> OldClass = TileClasses[Index];

		FVector Location = OldTile->GetActorLocation();
		FRotator Rotation = OldTile->GetActorRotation();

		OldTile->Destroy();

		TSubclassOf<AActor> Replacement = nullptr;

		ATileBase* OldTileBase = Cast<ATileBase>(OldTile);
		Replacement = OldTileBase -> ReplacementClass;
		
		if (!Replacement)
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid replacement for %s"), *OldClass->GetName());
			continue;
		}
		
		AActor* NewTile = GetWorld()->SpawnActor<AActor>(Replacement, Location, Rotation);
		Spawned[Index] = NewTile;

		RevertTileDataList.Add({ Location, Rotation, OldClass });
	}
	
	

	FTimerHandle RevertHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RevertHandle,
		FTimerDelegate::CreateUObject(this, &AShadowTileGimmickManager::RevertTiles, RevertTileDataList),
		5.0f,
		false
	);
}

void AShadowTileGimmickManager::RevertTiles(TArray<FRevertTileData> TileList)
{
	for (const FRevertTileData& Data : TileList)
	{
		GetWorld()->SpawnActor<AActor>(Data.OriginalClass, Data.Location, Data.Rotation);
	}
}