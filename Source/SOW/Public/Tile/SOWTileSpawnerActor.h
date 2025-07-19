#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOWTileSpawnerActor.generated.h"
class AEnemyIncomingRoute;

USTRUCT(BlueprintType)
struct FIncomingRouteDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route Definition")
	TArray<int32> TileIndices;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route Definition")
	FString RouteName;

	FIncomingRouteDefinition() : RouteName(TEXT("New Route")) {}
};

UCLASS()
class SOW_API ATileSpawner : public AActor
{
	GENERATED_BODY()

	TMap<int32, FVector> SpawnedTileLocations;

	UPROPERTY()
	TArray<AEnemyIncomingRoute*> SpawnedEnemyRoutes;
	
	void SpawnIncomingRoutes();

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AActor>> GridTiles;
	
	UPROPERTY(EditAnywhere)
	int32 GridWidth = 10;
	
	UPROPERTY(EditAnywhere)
	int32 GridHeight = 10;
	
	UPROPERTY(EditAnywhere)
	float TileWidth = 249.0f;
	
	UPROPERTY(EditAnywhere)
	float TileHeight = 249.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyIncomingRoute> EnemyIncomingRouteClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Spawner|Routes Configuration", meta = (TitleProperty = "RouteName"))
	TArray<FIncomingRouteDefinition> CustomIncomingRoutes;
	
	TArray<AEnemyIncomingRoute*> GetSpawnedEnemyRoutes() const;

	const TMap<int32, FVector>& GetSpawnedTileLocations() const { return SpawnedTileLocations; }
};