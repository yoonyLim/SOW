// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileBase.h"

// Sets default values
ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATileBase::OnTransformToAlternate() {}
void ATileBase::OnRevertToOriginal() {}

