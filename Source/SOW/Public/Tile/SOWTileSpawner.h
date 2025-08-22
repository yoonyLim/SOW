// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Actor.h"
#include "Tile/SOWTileSpawnerActor.h"
#include "Engine/EngineTypes.h"
#include "SOWTileSpawner.generated.h"

class AEnemyIncomingRoute;

UCLASS()
class SOW_API ASOWTileSpawner : public AActor
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

	UPROPERTY(EditAnywhere, Category = "BackgroundGradient")
	bool bSpawnGradientPlane = true;

	UPROPERTY(EditAnywhere, Category = "BackgroundGradient")
	UStaticMesh* GradientPlaneMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "BackgroundGradient")
	UMaterialInterface* GradientMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "BackgroundGradient")
	float GradientSoftnessWorld = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "BackgroundGradient")
	float PlaneZOffset = -1.0f;

	UPROPERTY(EditAnywhere, Category = "MapScript")
	bool bUseExternalMapScript = true;

	UPROPERTY(EditAnywhere, Category = "MapScript", meta=(FilePathFilter="ini"))
	FFilePath MapScriptFile;

	UFUNCTION(BlueprintCallable, Category = "MapScript")
	bool LoadAndBuildFromScript(const FString& InFilePathRelOrAbs);

	//UFUNCTION(Exec)
	//void ReloadMapScript();

private:
	UPROPERTY(Transient)
	AStaticMeshActor* GradientPlaneActor = nullptr;

	void SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY);
	static void ComputeMapBoundsFromTiles(const TMap<int32, FVector>& TileCenters, float TileWidth, float TileHeight, FVector& OutCenterWS, FVector2D& OutHalfSizeWorldXY);

	static bool ResolveMapScriptPath(const FString& Input, FString& OutAbsPath);
	
	void ClearSpawnedTiles();
	void ClearSpawnedRoutes();
};
