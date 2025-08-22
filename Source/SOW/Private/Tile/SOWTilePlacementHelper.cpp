// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/SOWTilePlacementHelper.h"

FVector SOWTilePlacementHelper::GetTileWorldPosition(int32 GridX, int32 GridY, float TileWidth, float TileHeight)
{
	float WorldX = GridX * TileWidth;
	float WorldY = GridY * TileHeight;
	return FVector(WorldX, WorldY, 10.0f);
}
