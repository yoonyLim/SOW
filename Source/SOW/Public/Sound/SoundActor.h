// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundActor.generated.h"

class UAudioComponent;

UCLASS()
class SOW_API ASoundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* BGMComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


