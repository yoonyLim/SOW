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
	if (!TileSpawnerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("TileSpawnerRef is nullptr! Check if ATileSpawner exists in the level."));
		return;
	}
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

			if (!(TileClasses.IsValidIndex(TileD) && TileClasses[TileA] && TileClasses[TileB] && TileClasses[TileC] && TileClasses[TileD] &&
				Spawned.IsValidIndex(TileD) && Spawned[TileA] && Spawned[TileB] && Spawned[TileC] && Spawned[TileD]))
			{
				continue;
			}
			const TArray<TSubclassOf<AActor>> TargetTiles = {
				TileClasses[TileA],  TileClasses[TileB],  TileClasses[TileC],  TileClasses[TileD]
			};
			bool bContainsExcludedName = false;
			for (const TSubclassOf<AActor> TargetActor : TargetTiles)
			{
				const FString ClassName = TargetActor -> GetName();
				if (ClassName.ToLower().Contains("blank") ||ClassName.ToLower().Contains("2x2"))
				{
					bContainsExcludedName = true;
					break;
				}
			}
			if (bContainsExcludedName)
			{
				continue;
			}
			validCoords.Add(FIntPoint(X, Y));
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
		TSubclassOf<AActor> OldClass = OldTile->GetClass();;

		FVector Location = OldTile->GetActorLocation();
		FRotator Rotation = OldTile->GetActorRotation();

		ATileBase* OldTileBase = Cast<ATileBase>(OldTile);
		
		if (!OldTileBase)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile at index %d is not ATileBase. Skipping transformation."), Index);
			continue;
		}

		TSubclassOf<AActor> Replacement = OldTileBase->ReplacementClass;
		
		OldTile->Destroy();
		
		
		if (!Replacement)
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid replacement for %s"), *OldClass->GetName());
			continue;
		}
		
		AActor* NewTile = GetWorld()->SpawnActor<AActor>(Replacement, Location, Rotation);
		Spawned[Index] = NewTile;
		UE_LOG(LogTemp, Log, TEXT("Tile at index %d replaced: %s -> %s"), Index, *OldClass->GetName(), *Replacement->GetName());
		RevertTileDataList.Add({ Location, Rotation, OldClass, Index});
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
	auto& Spawned = TileSpawnerRef->SpawnedTileActors;
	for (const FRevertTileData& Data : TileList)
	{
		Spawned[Data.SpawnIndex]->Destroy();
		AActor* Reverted = GetWorld()->SpawnActor<AActor>(Data.OriginalClass, Data.Location, Data.Rotation);
		Spawned[Data.SpawnIndex] = Reverted;
		UE_LOG(LogTemp, Log, TEXT("Tile at index %d reverted back to %s"), Data.SpawnIndex, *Data.OriginalClass->GetName());
	}
}