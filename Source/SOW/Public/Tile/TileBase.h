// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SOWEnumTypes.h"

#include "Tile/TileArcheType.h"

#include "TileBase.generated.h"

UCLASS()
class SOW_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileBase();
	virtual void OnConstruction(const FTransform& Transform) override;

	//  Added by hjy for new tile map system_25_10_08
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Config")
	ETileRole TileRole = ETileRole::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Config")
	float TileSizeUU = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Config")
	ETileZPolicy ZPolicy = ETileZPolicy::SnapToLandscape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Config", meta = (EditCondition="ZPolicy==ETileZPolicy::HoverRelative"))
	float HoverZ = 0.f;

	// [Patch] visual
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	UStaticMesh* VisualMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	UMaterialInterface* VisualMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	bool bHiddenVisual = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	bool bUseUniformScale = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual", meta=(EditCondition="bUseUniformScale"))
	float UniformScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual", meta = (EditCondition = "!bUseUniformScale"))
	FVector VisualScale3D = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	FVector2D VisualOffsetXY = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	float VisualZOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Visual")
	float VisualYawDeg = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|Grid") int32 GridX = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|Grid") int32 GridY = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile|State") bool bReserved = false;

	// [Patch] END

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATileBase> ReplacementClass;
	
	TSubclassOf<ATileBase> OriginalClass;
	virtual void OnTransformToAlternate();
	virtual void OnRevertToOriginal();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	USceneComponent* SceneRoot = nullptr;
	
	UPROPERTY(EditAnywhere)
	FVector TileSize = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile|Overlay") 
	UMaterialInterface* OverlayMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile|Overlay")
	UStaticMeshComponent* OverlayPlane;

	UPROPERTY()
	UMaterialInstanceDynamic* OverlayMID = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Summon")
	bool bCanPlace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon")
	ETileSummonState TileState;

	///  added by pgh
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowRange();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideRange();
	///  added by pgh

public:
	// [Patch] API
	UFUNCTION(BlueprintCallable, Category = "Tile|Prefab")
	void ConfigureFromData(UTileArcheType* Data);
	UFUNCTION(BlueprintCallable, Category = "Tile|Prefab")
	void ApplyZPolicy();
	UFUNCTION(BlueprintCallable, Category = "Tile|Visual")
	void ApplyVisualTransform();

	UFUNCTION(BlueprintCallable, Category = "Tile|Role")
	void SetRole(ETileRole NewRole);
	UFUNCTION(BlueprintPure, Category = "Tile|Role")
	bool IsBuildable() const { return TileRole == ETileRole::Buildable && !bReserved;  }
	UFUNCTION(BlueprintCallable, Category ="Tile|Role")
	void SetReserved(bool bInReserved) { bReserved = bInReserved; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void ShowTileMask();

	UFUNCTION(BlueprintCallable)
	void DeActivateTileMask();
};
