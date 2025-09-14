// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyIncomingRoute.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AEnemyIncomingRoute::AEnemyIncomingRoute()
{
	PrimaryActorTick.bCanEverTick = false;
	
	IncomingRoute = CreateDefaultSubobject<USplineComponent>("Incoming Route");
	SetRootComponent(IncomingRoute);
	IncomingRoute->SetMobility(EComponentMobility::Static);
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

void AEnemyIncomingRoute::SetSplinePointsFromLocations(const TArray<FVector>& Locations)
{
	if (!IncomingRoute)
		return;

	for (USplineMeshComponent* MeshComp : SplineMeshComponents)
	{
		if (MeshComp)
			MeshComp->DestroyComponent();
	}

	SplineMeshComponents.Empty();

	IncomingRoute->ClearSplinePoints(true);

	for (const FVector& Location : Locations)
	{
		FVector NewLocation = FVector(Location.X, Location.Y, Location.Z + 10.f);
		IncomingRoute->AddSplinePoint(NewLocation, ESplineCoordinateSpace::World, true);
	}

	IncomingRoute->UpdateSpline();

	if (!RouteMesh || GetNumberOfPoints() < 2)
		return;

	for (int32 i = 0; i < GetNumberOfPoints() - 2; i++)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Spline Points: %d"), i);
		
		// Create the spline mesh component
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (!SplineMesh)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Failed to create Spline Mesh"));
			continue;
		}

		// Set the static mesh to be used
		SplineMesh->SetStaticMesh(RouteMesh);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::X, true);

		SplineMesh->SetStartScale(FVector2D(0.1f, 0.1f), true);
		SplineMesh->SetEndScale(FVector2D(0.1f, 0.1f), true);

		// Register and attach it to our main spline component
		SplineMesh->RegisterComponent();
		SplineMesh->AttachToComponent(IncomingRoute, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMesh->SetMobility(EComponentMobility::Static); // Match parent mobility

		// Get the start and end points for this segment from the main spline
		const FVector StartPoint = IncomingRoute->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector StartTangent = IncomingRoute->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndPoint = IncomingRoute->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = IncomingRoute->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		// Configure the spline mesh with the start/end points and tangents
		SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
        
		// Add the new component to our array for tracking
		SplineMeshComponents.Add(SplineMesh);
	}
}

