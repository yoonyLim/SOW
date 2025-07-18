// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyIncomingRoute.h"

#include "Components/SplineComponent.h"


// Sets default values
AEnemyIncomingRoute::AEnemyIncomingRoute()
{
	IncomingRoute = CreateDefaultSubobject<USplineComponent>("Incoming Route");
	SetRootComponent(IncomingRoute);
	IncomingRoute->SetMobility(EComponentMobility::Movable);
}

int AEnemyIncomingRoute::GetNumberOfPoints() const
{
	return IncomingRoute->GetNumberOfSplinePoints();
}

FVector AEnemyIncomingRoute::GetCurrentIncomingIndexPosition(const int32 Index) const
{
	if (IncomingRoute && IncomingRoute->GetNumberOfSplinePoints() > Index)
		return IncomingRoute->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return FVector::ZeroVector;
}

void AEnemyIncomingRoute::SetSplinePointsFromLocations(const TArray<FVector>& Locations) const
{
	if (!IncomingRoute)
		return;

	IncomingRoute->ClearSplinePoints(true);

	for (const FVector& Location : Locations)
		IncomingRoute->AddSplinePoint(Location, ESplineCoordinateSpace::World, true);

	IncomingRoute->UpdateSpline();
}

