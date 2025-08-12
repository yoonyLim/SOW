// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Core/SOWPlayerController.h"

// Sets default values
ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	OverlayPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OverlayPlane"));
	OverlayPlane->SetupAttachment(MeshComponent);           

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMesh.Succeeded())
	{
		OverlayPlane->SetStaticMesh(PlaneMesh.Object);
	}

	OverlayPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlayPlane->SetCastShadow(false);
	OverlayPlane->SetRelativeLocation(FVector(0, 0, 0.1f));    
	OverlayPlane->SetRelativeRotation(FRotator(0.f, 45.f, 0.f));
	OverlayPlane->SetVisibility(false);
}

void ATileBase::BeginPlay()
{
	Super::BeginPlay();

	if (!OverlayMID)
	{
		OverlayMID = OverlayPlane->CreateAndSetMaterialInstanceDynamic(0);
	}

	OverlayMID->SetScalarParameterValue(TEXT("UseInstanceData"), 0.f);
	OverlayMID->SetScalarParameterValue(TEXT("CanPlace"), bCanPlace ? 1.f : 0.f);

	ASOWPlayerController* PC = Cast<ASOWPlayerController>(GetWorld()->GetFirstPlayerController());

	PC->SummonEnd.AddDynamic(this, &ATileBase::DeActivateTileMask);
	PC->SummonStart.AddDynamic(this, &ATileBase::ShowTileMask);
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
	UE_LOG(LogTemp, Error, TEXT("DD"));
	OverlayMID->SetScalarParameterValue(TEXT("CanPlace"), bCanPlace ? 1.f : 0.f);
	OverlayPlane->SetVisibility(true);
}

void ATileBase::DeActivateTileMask()
{
	OverlayPlane->SetVisibility(false);
}