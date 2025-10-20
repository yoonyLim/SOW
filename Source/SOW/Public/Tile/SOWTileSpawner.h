// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Actor.h"
#include "Tile/SOWTileSpawnerActor.h"
#include "Engine/EngineTypes.h"
#include "Tile/TileBase.h"
#include "Tile/TileArcheType.h"
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
	UPROPERTY(EditAnywhere, Category = "Grid", meta = (ClampMin = "1"))
	int32 GridWidth = 16;

	UPROPERTY(EditAnywhere, Category = "Grid", meta = (ClampMin = "1"))
	int32 GridHeight = 10;

	UPROPERTY(EditAnywhere, Category = "Grid", meta = (ClampMin = "1"))
	float TileWidth = 300.f;

	UPROPERTY(EditAnywhere, Category = "Grid", meta = (ClampMin = "1"))
	float TileHeight = 300.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<class AActor>> GridTiles;

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

	UPROPERTY(EditAnywhere, Category = "Grid|Prefab")
	bool bUseArcheTypeAfterSpawn = false;

	UPROPERTY(EditAnywhere, Category = "Grid|Prefab")
	TMap<FName, TSoftObjectPtr<UTileArcheType>> TokenToArcheType;

	UFUNCTION(BlueprintCallable, Category = "MapScript")
	bool LoadAndBuildFromScript(const FString& InFilePathRelOrAbs);

	UFUNCTION(BlueprintCallable, Category = "MapScript")
	void SpawnIncomingRoutes();

	//UFUNCTION(Exec)
	//void ReloadMapScript();

private:
	UPROPERTY(Transient)
	AStaticMeshActor* GradientPlaneActor = nullptr;

	UPROPERTY(Transient)
	TArray<FIncomingRouteDefinition> RoutesFromScript;

	FVector GridToWorld_TopLeft(int32 GX, int32 GY, float InTileW, float InTileH) const;

private:

	void SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY);
	static void ComputeMapBoundsFromTiles(const TMap<int32, FVector>& TileCenters, float TileWidth, float TileHeight, FVector& OutCenterWS, FVector2D& OutHalfSizeWorldXY);

	static bool ResolveMapScriptPath(const FString& Input, FString& OutAbsPath);
	
	ETileRole ResolveRoleFromToken(const FName& Token) const;

	void ApplyArcheTypeIfSet(AActor* SpawnedTile, int32 X, int32 Y, const FName& UseToken, float InTileWidth);


	void ClearSpawnedTiles();
	void ClearSpawnedRoutes();
};
