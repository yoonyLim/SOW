// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridTileInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGridTileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOW_API IGridTileInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = GridTileInterface)
	void SetTileEnemySpawnerIndex(int32 NewIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = GridTileInterface)
	int32 GetTileEnemySpawnerIndex() const;
};
