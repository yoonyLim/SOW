// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void ATileBase::OnTransformToAlternate() {}
void ATileBase::OnRevertToOriginal() {}

void ATileBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	MeshComponent->SetRelativeScale3D(TileSize);
}