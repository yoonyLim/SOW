#include "Tile/SOWTileSpawnerActor.h"
#include "Tile/SOWTilePlacementHelper.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "Interface/GridTileInterface.h"
#include "Utilities/EnemyIncomingRoute.h"

void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnedTileLocations.Empty();
	SpawnedEnemyRoutes.Empty();

	int32 EnemySpawnerIndexToSpawn = 0;

	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			const int32 Index = Y * GridWidth + X;

			if (!GridTiles.IsValidIndex(Index) || !GridTiles[Index])
			{
				continue;
			}

			FVector SpawnLocation = SOWTilePlacementHelper::GetTileWorldPosition(X, Y, TileWidth, TileHeight);
			
			if (GridTiles[Index] && GridTiles[Index]->GetName().Contains(TEXT("2x2")))
			{
				SpawnLocation += FVector( 0, TileHeight, 0.0f);
			}
			
			FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

			if (AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation))
			{
				SpawnedTileActors.Add(SpawnedTile);
				SpawnedTileLocations.Add(Index, SpawnLocation);

				if (SpawnedTile->Implements<UGridTileInterface>())
				{
					if (EnemySpawnerIndexToSpawn < EnemySpawnerIndexInOrder.Num() && GridTiles[Index]->GetName().Contains(TEXT("Enemy")))
					{
						IGridTileInterface::Execute_SetTileEnemySpawnerIndex(SpawnedTile, EnemySpawnerIndexInOrder[EnemySpawnerIndexToSpawn]); // Call the interface function
						// UE_LOG(LogTemp, Error, TEXT("Enemy Spawner '%d' set."), EnemySpawnerIndexInOrder[EnemySpawnerIndexToSpawn]);
						EnemySpawnerIndexToSpawn++;
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Enemy Spawner failed to set."));
					}
				}
			}
		}
	}

	SpawnIncomingRoutes();
	
	const FVector Center = SOWTilePlacementHelper::GetTileWorldPosition((GridWidth - 1) / 2.0f, (GridHeight - 1) / 2.0f, TileWidth, TileHeight);
	const FVector Extent = FVector(
		(GridWidth * TileWidth * 0.5f) + TileWidth,
		(GridHeight * TileHeight * 0.5f) + TileHeight,
		2000.0f
	);

	ANavMeshBoundsVolume* Volume = nullptr;
	for (TActorIterator<ANavMeshBoundsVolume> It(GetWorld()); It; ++It)
	{
		Volume = *It;
		break;
	}

	if (!Volume)
	{
		FActorSpawnParameters SpawnParams;
		Volume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), Center, FRotator::ZeroRotator, SpawnParams);
	}

	if (Volume)
	{
		Volume->SetActorLocation(Center);
		Volume->SetActorScale3D(FVector(1.0f));
		
		if (Volume->GetRootComponent())
			Volume->GetRootComponent()->SetWorldScale3D(Extent);
	}

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->Build();
	}
}

void ATileSpawner::SpawnIncomingRoutes()
{
	if (!EnemyIncomingRouteClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyIncomingRouteClass is not set in ATileSpawner! Cannot spawn enemy routes."));
		return;
	}
	
	SpawnedEnemyRoutes.Empty(); 

	for (int32 RouteIndex = 0; RouteIndex < CustomIncomingRoutes.Num(); ++RouteIndex)
	{
		const FIncomingRouteDefinition& CurrentRouteDefinition = CustomIncomingRoutes[RouteIndex];
		const TArray<int32>& CurrentRouteIndices = CurrentRouteDefinition.TileIndices;
		TArray<FVector> CurrentRouteWorldLocations;

		for (const int32 TileIndex : CurrentRouteIndices)
		{
			if (SpawnedTileLocations.Contains(TileIndex))
			{
				CurrentRouteWorldLocations.Add(SpawnedTileLocations[TileIndex]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Tile index %d for route %d was not found in SpawnedTileLocations. Route may be incomplete."), TileIndex, RouteIndex);
			}
		}

		if (CurrentRouteWorldLocations.Num() > 1) // at least two points for a spline
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;

			AEnemyIncomingRoute* NewRouteActor = GetWorld()->SpawnActor<AEnemyIncomingRoute>(
				EnemyIncomingRouteClass, 
				FVector::ZeroVector, // Location doesn't matter much relative to spline points
				FRotator::ZeroRotator, 
				SpawnParams
			);

			if (NewRouteActor)
			{
				NewRouteActor->SetSplinePointsFromLocations(CurrentRouteWorldLocations);
				NewRouteActor->SetActorLabel(FString::Printf(TEXT("EnemyRoute_%d"), RouteIndex));
				SpawnedEnemyRoutes.Add(NewRouteActor);
				UE_LOG(LogTemp, Log, TEXT("Spawned EnemyIncomingRoute Actor for Route %d with %d points. Total routes: %d"), 
					RouteIndex, CurrentRouteWorldLocations.Num(), SpawnedEnemyRoutes.Num());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn AEnemyIncomingRoute actor for Route %d."), RouteIndex);
			}
		}
		else if (CurrentRouteWorldLocations.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Route %d has only %d point(s). Skipping spline creation (requires > 1 point)."), RouteIndex, CurrentRouteWorldLocations.Num());
		}
	}
}

TArray<AEnemyIncomingRoute*> ATileSpawner::GetSpawnedEnemyRoutes() const
{
	return SpawnedEnemyRoutes;
}