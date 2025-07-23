// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyIncomingRoute.generated.h"

class USplineComponent;

UCLASS()
class SOW_API AEnemyIncomingRoute : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USplineComponent* IncomingRoute;

public:
	// Sets default values for this actor's properties
	AEnemyIncomingRoute();

public:
	// Called every frame
	int GetNumberOfPoints() const;
	FVector GetCurrentIncomingIndexPosition(const int32 Index) const;
	void SetSplinePointsFromLocations(const TArray<FVector>& Locations) const;
};
