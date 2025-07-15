// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileBase.h"
#include "GameFramework/Actor.h"
#include "ShadowTileGimmickManager.generated.h"

UCLASS()
class SOW_API AShadowTileGimmickManager : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	FTimerHandle TransformTimerHandle;

	void PickRandomTileAndTransform();

	void RevertTile(FVector Location, FRotator Rotation, TSubclassOf<ATileBase> OriginalClass);
};
