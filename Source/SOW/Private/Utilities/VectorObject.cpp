// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/VectorObject.h"


void UVectorObject::SetSelectedLocation(FVector InVector)
{
	SelectedLocation = InVector;
}

FVector UVectorObject::GetAxisLocation() const
{
	float X = FMath::Abs(SelectedLocation.X);
	float Y = FMath::Abs(SelectedLocation.Y);

	X = X >= Y ? X : 0.f;
	Y = X < Y ? Y : 0.f;

	return FVector(X,Y,0.f);
}
