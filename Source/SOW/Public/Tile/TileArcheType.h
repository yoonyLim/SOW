// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileArcheType.generated.h"

UENUM(BlueprintType)
enum class ETileRole : uint8
{
	Empty		UMETA(DisplayName ="Empty"),
	Path		UMETA(DisplayName = "Path"),
	Buildable	UMETA(DisplayName = "Buildable"),
	Blocked		UMETA(DisplayName = "Blocked"),
	Spawn		UMETA(DisplayName = "Spawn"),
	Goal		UMETA(DisplayName = "Goal"),
};

UENUM(BlueprintType)
enum class ETileZPolicy : uint8
{
	SnapToLandscape		UMETA(DisplayName = "SnapToLandscape"),
	HoverRelative		UMETA(DisplayName = "HoverRelative"),
	AbsoluteZ	UMETA(DisplayName = "AbsoluteZ"),
};

/**
 * 
 */
UCLASS()
class SOW_API UTileArcheType : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) ETileRole Role = ETileRole::Empty;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float TileSizeUU = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) ETileZPolicy ZPolicy = ETileZPolicy::SnapToLandscape;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "ZPolicy == ETileZPolicy::HoverRelative")) float HoverZ = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) UMaterialInterface* Material = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bHiddenVisual = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bUseUniformScale = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bUseUniformScale")) float UniformScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "!bUseUniformScale")) FVector VisualScale3D = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FVector2D VisualOffsetXY = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float VisualZOffset = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float VisualYawDeg = 0.f;
};
