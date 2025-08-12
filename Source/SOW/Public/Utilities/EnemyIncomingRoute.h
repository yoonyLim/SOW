// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyIncomingRoute.generated.h"

class USplineMeshComponent;
class USplineComponent;

UCLASS()
class SOW_API AEnemyIncomingRoute : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<USplineMeshComponent>> SplineMeshComponents;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	TObjectPtr<USplineComponent> IncomingRoute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	TObjectPtr<UStaticMesh> RouteMesh;
	
	// Sets default values for this actor's properties
	AEnemyIncomingRoute();
	
	int GetNumberOfPoints() const;
	FVector GetCurrentIncomingIndexPosition(const int32 Index) const;
	void SetSplinePointsFromLocations(const TArray<FVector>& Locations);
};
