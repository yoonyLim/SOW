// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile/TileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Core/SOWPlayerController.h"

// Sets default values
ATileBase::ATileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetMobility(EComponentMobility::Movable);

	OverlayPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OverlayPlane"));
	OverlayPlane->SetupAttachment(SceneRoot);
	OverlayPlane->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMesh.Succeeded())
	{
		OverlayPlane->SetStaticMesh(PlaneMesh.Object);
	}

	FVector V = MeshComponent->GetRelativeLocation();
	V.Z += 0.01;

	OverlayPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlayPlane->SetCastShadow(false);
	OverlayPlane->SetRelativeLocation(V);
	OverlayPlane->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	OverlayPlane->SetVisibility(false);
}

void ATileBase::ConfigureFromData(UTileArcheType* Data)
{
	UE_LOG(LogTemp, Log, TEXT("TileBase: Match ArcheType Data "));

	if (!Data) return;
	TileRole = Data->Role;
	TileSizeUU = Data->TileSizeUU;
	ZPolicy = Data->ZPolicy;
	HoverZ = Data->HoverZ;

	VisualMesh = Data->Mesh;
	VisualMaterial = Data->Material;
	bHiddenVisual = Data->bHiddenVisual;

	bUseUniformScale = Data->bUseUniformScale;
	UniformScale = Data->UniformScale;
	VisualScale3D = Data->VisualScale3D;
	VisualOffsetXY = Data->VisualOffsetXY;
	VisualZOffset = Data->VisualZOffset;
	VisualYawDeg = Data->VisualYawDeg;

	if (VisualMesh)     MeshComponent->SetStaticMesh(VisualMesh);
	if (VisualMaterial) MeshComponent->SetMaterial(0, VisualMaterial);
	MeshComponent->SetHiddenInGame(bHiddenVisual);

	SetRole(TileRole);
	ApplyZPolicy();
	ApplyVisualTransform();
}

void ATileBase::SetRole(ETileRole NewRole)
{
	TileRole = NewRole;

	if (OverlayMID)
	{
		OverlayMID->SetScalarParameterValue(TEXT("UseInstanceData"), 0.f);
		OverlayMID->SetScalarParameterValue(TEXT("Role"), static_cast<float>((uint8)TileRole));
		OverlayMID->SetScalarParameterValue(TEXT("CanPlace"), (TileRole == ETileRole::Buildable && !bReserved) ? 1.f : 0.f);
	}

	if (OverlayPlane)
	{
		const float S = FMath::Max(0.01f, TileSizeUU / 100.f);
		OverlayPlane->SetRelativeScale3D(FVector(S, S, 1.f));
	}
}

void ATileBase::ApplyZPolicy()
{
	FVector L = GetActorLocation();
	switch (ZPolicy)
	{
	case ETileZPolicy::SnapToLandscape:
	case ETileZPolicy::HoverRelative:
	{
		UWorld* W = GetWorld(); if (!W) break;
		const FVector Start = L + FVector(0, 0, 3000);
		const FVector End = L - FVector(0, 0, 10000);
		FHitResult Hit;
		if (W->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
		{
			float Z = Hit.Location.Z + 0.1f;
			if (ZPolicy == ETileZPolicy::HoverRelative) Z += HoverZ;
			SetActorLocation(FVector(L.X, L.Y, Z));
		}
		break;
	}
	case ETileZPolicy::AbsoluteZ:
	default: break;
	}
}

void ATileBase::ApplyVisualTransform()
{
	if (!MeshComponent) return;

	const FVector BaseScale = MeshComponent->GetRelativeScale3D();
	const FVector Scale = bUseUniformScale ? FVector(UniformScale) : VisualScale3D;
	MeshComponent->SetRelativeScale3D(Scale); 

	MeshComponent->SetRelativeLocation(FVector(VisualOffsetXY.X, VisualOffsetXY.Y, VisualZOffset));
	MeshComponent->SetRelativeRotation(FRotator(0.f, VisualYawDeg, 0.f));
}

//void ATileBase::ShowRange()
//{
//	checkf(OverlayMID, TEXT("OverlayMID not assigned"));
//	checkf(OverlayPlane, TEXT("OverlayPlane not assigned"));
//	//if (!IsValid(OverlayMID) || !IsValid(OverlayPlane)) return;
//
//	OverlayMID->SetScalarParameterValue(TEXT("CanPlace"), bCanPlace ? 1.f : 0.f);
//	OverlayPlane->SetVisibility(true);
//}
//
//void ATileBase::HideRange()
//{
//	checkf(OverlayMID, TEXT("OverlayMID not assigned"));
//	checkf(OverlayPlane, TEXT("OverlayPlane not assigned"));
//	OverlayPlane->SetVisibility(false);
//}

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

	if (!bCanPlace)
	{
		TileState = ETileSummonState::Unavailable;
	}
	else
	{
		TileState = ETileSummonState::Available;
	}

	ApplyVisualTransform();
}

void ATileBase::OnTransformToAlternate() {}
void ATileBase::OnRevertToOriginal() {}

void ATileBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (VisualMesh) MeshComponent->SetStaticMesh(VisualMesh);
	if (VisualMaterial) MeshComponent->SetMaterial(0, VisualMaterial);
	MeshComponent->SetHiddenInGame(bHiddenVisual);

	SetRole(TileRole);
	ApplyZPolicy();
	ApplyVisualTransform();
}

void ATileBase::ShowTileMask()
{
	OverlayMID->SetScalarParameterValue(TEXT("CanPlace"), bCanPlace ? 1.f : 0.f);
	OverlayPlane->SetVisibility(true);
}

void ATileBase::DeActivateTileMask()
{
	OverlayPlane->SetVisibility(false);
}