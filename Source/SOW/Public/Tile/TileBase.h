// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SOWEnumTypes.h"
#include "TileBase.generated.h"

UCLASS()
class SOW_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATileBase> ReplacementClass;
	
	TSubclassOf<ATileBase> OriginalClass;
	virtual void OnTransformToAlternate();
	virtual void OnRevertToOriginal();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	UStaticMeshComponent* MeshComponent;
	
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
	UFUNCTION(BlueprintCallable)
	void ShowRange();
	UFUNCTION(BlueprintCallable)
	void HideRange();
	///  added by pgh

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void ShowTileMask();

	UFUNCTION(BlueprintCallable)
	void DeActivateTileMask();
};
