// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere)
	FVector TileSize = FVector(1.0f, 1.0f, 1.0f);
};
