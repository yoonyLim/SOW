// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyIncomingRouteComponent.generated.h"


class AEnemyIncomingRoute;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOW_API UEnemyIncomingRouteComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	AEnemyIncomingRoute* EnemyIncomingRoute;

	int IncomingRouteIndex = 0;

public:
	UFUNCTION(BlueprintCallable)
	void SetIncomingRoute(AEnemyIncomingRoute* IncomingRoute);
	
	bool IncrementIncomingRouteIndex(); // return true if reached the end to freely head toward the core rune
	FVector GetCurrentIndexPosition() const;
	AEnemyIncomingRoute* GetIncomingRoute() const { return EnemyIncomingRoute; };
};
