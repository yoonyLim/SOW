// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Enemies/EnemyIncomingRouteComponent.h"

#include "Utilities/EnemyIncomingRoute.h"

void UEnemyIncomingRouteComponent::SetIncomingRoute(AEnemyIncomingRoute* NewIncomingRoute)
{
	EnemyIncomingRoute = NewIncomingRoute;
}

bool UEnemyIncomingRouteComponent::IncrementIncomingRouteIndex()
{
	IncomingRouteIndex += 1;

	if (IncomingRouteIndex > EnemyIncomingRoute->GetNumberOfPoints() - 1)
		return true;

	return false;
}

FVector UEnemyIncomingRouteComponent::GetCurrentIndexPosition() const
{
	return EnemyIncomingRoute->GetCurrentIncomingIndexPosition(IncomingRouteIndex);
}
