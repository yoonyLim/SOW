#include "Tile/SOWTileSpawnerActor.h"
#include "Tile/SOWTilePlacementHelper.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"


void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();

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
			
			FString ClassName = GridTiles[Index]->GetName();
			
			if (ClassName.Contains(TEXT("2x2")))
			{
				SpawnLocation += FVector( 0, TileHeight, 0.0f);
			}
			
			FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation);
			SpawnedTileActors.Add(Spawned);
		}
	}
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

	Volume->SetActorLocation(Center);
	Volume->SetActorScale3D(FVector(1.0f));
	Volume->GetRootComponent()->SetWorldScale3D(Extent);

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->Build();
	}
}
