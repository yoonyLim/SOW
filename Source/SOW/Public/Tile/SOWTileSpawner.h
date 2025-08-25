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
	TArray<TObjectPtr<AEnemyIncomingRoute>> SpawnedEnemyRoutes;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> SpawnedTileActors;

	UPROPERTY(EditAnywhere)
	TArray<int32> EnemySpawnerIndexInOrder;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyIncomingRoute> EnemyIncomingRouteClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Spawner|Routes Configuration", meta = (TitleProperty = "RouteName"))
	TArray<FIncomingRouteDefinition> CustomIncomingRoutes;

	TArray<TObjectPtr<AEnemyIncomingRoute>> GetSpawnedEnemyRoutes() const;

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

	UFUNCTION(BlueprintCallable, Category = "MapScript")
	void SpawnIncomingRoutes();

	//UFUNCTION(Exec)
	//void ReloadMapScript();

private:
	UPROPERTY(Transient)
	AStaticMeshActor* GradientPlaneActor = nullptr;

	int32 GridWidth = 9;

	int32 GridHeight = 5;

	float TileWidth = 116.f;

	float TileHeight = 116.f;

	TArray<TSubclassOf<class AActor>> GridTiles;

	UPROPERTY(Transient)
	TArray<FIncomingRouteDefinition> RoutesFromScript;

private:

	void SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY);
	static void ComputeMapBoundsFromTiles(const TMap<int32, FVector>& TileCenters, float TileWidth, float TileHeight, FVector& OutCenterWS, FVector2D& OutHalfSizeWorldXY);

	static bool ResolveMapScriptPath(const FString& Input, FString& OutAbsPath);
	
	void ClearSpawnedTiles();
	void ClearSpawnedRoutes();
};
