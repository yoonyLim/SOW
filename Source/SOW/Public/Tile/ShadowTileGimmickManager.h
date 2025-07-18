// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileBase.h"
#include "GameFramework/Actor.h"
#include "ShadowTileGimmickManager.generated.h"

USTRUCT()
struct FRevertTileData
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;
	TSubclassOf<AActor> OriginalClass;
};

UCLASS()
class SOW_API AShadowTileGimmickManager : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	FTimerHandle TransformTimerHandle;
	
	UPROPERTY()
	ATileSpawner* TileSpawnerRef;

	void PickRandomTileAndTransform();

	void RevertTiles(TArray<FRevertTileData> TileList);
};

