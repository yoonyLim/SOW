// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Manager/SummonManager.h"
#include "SOWGameInstance.h"

// Sets default values
ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlayPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OverlayPlane"));
	SetRootComponent(OverlayPlane);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(
		TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMesh.Succeeded())
	{
		OverlayPlane->SetStaticMesh(PlaneMesh.Object);
	}

	OverlayPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlayPlane->SetCastShadow(false);
	OverlayPlane->SetRelativeLocation(FVector(0, 0, 0.1));
	OverlayPlane->SetRelativeRotation(FRotator(0.f, 45.f, 0.f));
	OverlayPlane->SetVisibility(false);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void ATileBase::BeginPlay()
{
	Super::BeginPlay();
	
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());

	USummonManager* SM = GI->GetSummonManager();

	SM->SummonStart.AddDynamic(this, &ATileBase::ShowTileMask);
	SM->SummonEnd.AddDynamic(this, &ATileBase::DeActivateTileMask);

	if (!OverlayMID)
	{
		OverlayMID = OverlayPlane->CreateAndSetMaterialInstanceDynamic(0);
	}

	OverlayMID->SetScalarParameterValue(TEXT("UseInstanceData"), 0.f);
	OverlayMID->SetScalarParameterValue(TEXT("CanPlace"), bCanPlace ? 1.f : 0.f);
}

void ATileBase::OnTransformToAlternate() {}
void ATileBase::OnRevertToOriginal() {}

void ATileBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	MeshComponent->SetRelativeScale3D(TileSize);
}

void ATileBase::ShowTileMask()
{
	OverlayPlane->SetVisibility(true);
}

void ATileBase::DeActivateTileMask()
{
	OverlayPlane->SetVisibility(false);
}