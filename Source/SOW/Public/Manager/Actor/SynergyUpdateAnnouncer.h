// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SynergyUpdateAnnouncer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSynergyUpdatedDelegate, EElementalType, ElementType, int, SynergyCount, const TArray<FName>&, SynergyComposer);
UCLASS()
class SOW_API ASynergyUpdateAnnouncer : public AActor
{
	GENERATED_BODY()
	
public:	
	ASynergyUpdateAnnouncer();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyUpdatedDelegate OnSynergyUpdated;
	// Sets default values for this actor's properties
	
};
