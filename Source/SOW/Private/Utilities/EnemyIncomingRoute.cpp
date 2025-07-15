// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyIncomingRoute.h"

#include "Components/SplineComponent.h"


// Sets default values
AEnemyIncomingRoute::AEnemyIncomingRoute()
{
	IncomingRoute = CreateDefaultSubobject<USplineComponent>("Incoming Route");
}

int AEnemyIncomingRoute::GetNumberOfPoints() const
{
	return IncomingRoute->GetNumberOfSplinePoints();
}

FVector AEnemyIncomingRoute::GetCurrentIncomingIndexPosition(int Index)
{
	return IncomingRoute->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

