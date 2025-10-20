// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VectorObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SOW_API UVectorObject : public UObject
{
	GENERATED_BODY()

	FVector SelectedLocation;
public:

	UFUNCTION(BlueprintCallable)
	void SetSelectedLocation(FVector InVector);

	UFUNCTION(BlueprintPure)
	FVector GetAxisLocation() const;
	
};
