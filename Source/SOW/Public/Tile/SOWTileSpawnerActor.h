#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInterface.h"
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
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> DefaultTileClass;
	
	// === 배경 그라데이션 평면 자동 생성 옵션 ===
	UPROPERTY(EditAnywhere, Category="BackgroundGradient")
	bool bSpawnGradientPlane = true;

	// Plane로 쓸 StaticMesh (기본: /Engine/BasicShapes/Plane)
	UPROPERTY(EditAnywhere, Category="BackgroundGradient")
	UStaticMesh* GradientPlaneMesh = nullptr;

	// 그라데이션 머티리얼 (TransformPosition(World→Local) 버전)
	UPROPERTY(EditAnywhere, Category="BackgroundGradient")
	UMaterialInterface* GradientMaterial = nullptr;

	// 바깥 페이드 폭(월드 단위) — Target Plane 크기에 2*SoftnessWorld 더해줌
	UPROPERTY(EditAnywhere, Category="BackgroundGradient")
	float GradientSoftnessWorld = 1200.0f;

	// 평면 Z 오프셋 (타일 아래로 살짝)
	UPROPERTY(EditAnywhere, Category="BackgroundGradient")
	float PlaneZOffset = -1.0f;

private:
	// 생성된 평면 액터 핸들
	UPROPERTY(Transient)
	AStaticMeshActor* GradientPlaneActor = nullptr;

	void SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY);
	static void ComputeMapBoundsFromTiles(const TMap<int32, FVector>& TileCenters, float TileWidth, float TileHeight, FVector& OutCenterWS, FVector2D& OutHalfSizeWorldXY);
};