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

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> SpawnedTileActors;
	// 현재 맵에 스폰된 타일 인스턴스를 저장하는 배열
	// GridTiles[index]로 생성된 액터와 동일한 index
	// (x,y) 의 index = y * GridWidth + x
	
	UPROPERTY(EditAnywhere)
	int32 GridWidth = 10;
	
	UPROPERTY(EditAnywhere)
	int32 GridHeight = 10;
	
	UPROPERTY(EditAnywhere)
	float TileWidth = 124.5f;
	
	UPROPERTY(EditAnywhere)
	float TileHeight = 124.5f;

	UPROPERTY(EditAnywhere)
	TArray<int32> EnemySpawnerIndexInOrder;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyIncomingRoute> EnemyIncomingRouteClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Spawner|Routes Configuration", meta = (TitleProperty = "RouteName"))
	TArray<FIncomingRouteDefinition> CustomIncomingRoutes;
	
	TArray<AEnemyIncomingRoute*> GetSpawnedEnemyRoutes() const;

	const TMap<int32, FVector>& GetSpawnedTileLocations() const { return SpawnedTileLocations; }
};