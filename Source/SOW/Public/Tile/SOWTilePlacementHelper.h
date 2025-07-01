// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SOW_API SOWTilePlacementHelper
{
public:
	static FVector GetTileWorldPosition(int32 GridX, int32 GridY, float TileWidth, float TileHeight);
};
