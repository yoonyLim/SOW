#include "Tile/SOWTileSpawnerActor.h"
#include "Tile/SOWTilePlacementHelper.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "Utilities/EnemyIncomingRoute.h"

void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnedTileLocations.Empty();
	SpawnedEnemyRoutes.Empty();

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
			
			AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation);

			if (SpawnedTile)
				SpawnedTileLocations.Add(Index, SpawnLocation);
		}
	}

	DefineEnemyRoutes();

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

void ATileSpawner::DefineEnemyRoutes()
{
	DefinedIncomingRoutes.Empty();

	// --- Example Route Definitions ---
	// Route 1 (Top edge)
	TArray<int32> Route1;
	for (int32 X = 0; X < GridWidth; ++X)
	{
		Route1.Add(0 * GridWidth + X); // First row
	}
	DefinedIncomingRoutes.Add(Route1);

	// Route 2 (Left edge)
	TArray<int32> Route2;
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		Route2.Add(Y * GridWidth + 0); // First column
	}
	DefinedIncomingRoutes.Add(Route2);

	// Route 3 (Bottom edge)
	TArray<int32> Route3;
	for (int32 X = GridWidth - 1; X >= 0; --X) // From right to left
	{
		Route3.Add((GridHeight - 1) * GridWidth + X); // Last row
	}
	DefinedIncomingRoutes.Add(Route3);

	// Route 4 (Right edge)
	TArray<int32> Route4;
	for (int32 Y = GridHeight - 1; Y >= 0; --Y) // From bottom to top
	{
		Route4.Add(Y * GridWidth + (GridWidth - 1)); // Last column
	}
	DefinedIncomingRoutes.Add(Route4);
}

void ATileSpawner::SpawnIncomingRoutes()
{
	if (!EnemyIncomingRouteClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyIncomingRouteClass is not set in ATileSpawner! Cannot spawn enemy routes."));
		return;
	}
	
	SpawnedEnemyRoutes.Empty(); 

	for (int32 RouteIndex = 0; RouteIndex < DefinedIncomingRoutes.Num(); ++RouteIndex)
	{
		const TArray<int32>& CurrentRouteIndices = DefinedIncomingRoutes[RouteIndex];
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