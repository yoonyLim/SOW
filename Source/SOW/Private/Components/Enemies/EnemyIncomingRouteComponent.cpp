// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Enemies/EnemyIncomingRouteComponent.h"

#include "Utilities/EnemyIncomingRoute.h"


// Sets default values for this component's properties
UEnemyIncomingRouteComponent::UEnemyIncomingRouteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEnemyIncomingRouteComponent::SetIncomingRoute(AEnemyIncomingRoute* NewIncomingRoute)
{
	IncomingRoute = NewIncomingRoute;
}

bool UEnemyIncomingRouteComponent::IncrementIncomingRouteIndex()
{
	IncomingRouteIndex += 1;

	if (IncomingRouteIndex >= IncomingRoute->GetNumberOfPoints() - 1)
		return true;

	return false;
}

FVector UEnemyIncomingRouteComponent::GetCurrentIndexPosition() const
{
	return IncomingRoute->GetCurrentIncomingIndexPosition(IncomingRouteIndex);
}
