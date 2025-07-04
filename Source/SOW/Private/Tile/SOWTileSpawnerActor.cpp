#include "Tile/SOWTileSpawnerActor.h"
#include "Tile/SOWTilePlacementHelper.h"
#include "Kismet/GameplayStatics.h"

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
			FRotator Rotation = FRotator(0.0f, 90.0f, 0.0f);
			GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation);
		}
	}
}