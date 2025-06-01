// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"
#include "GameFramework/Actor.h"
#include "TurretMeleeHitCollision.generated.h"

class UBoxComponent;
class ASOWCharacterTurretBase;

UCLASS()
class SOW_API ATurretMeleeHitCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretMeleeHitCollision();

	void ToggleCollision(bool bShouldEnable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Combat")
	UBoxComponent* MeleeHitCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Combat", meta = (ExposeOnSpawn = true))
	ETurretTargetSelectionPolicy OwnerPolicy;

private:

	UFUNCTION()
	void MeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	ASOWCharacterTurretBase* CachedInstigator;

	TArray<AActor*> OverlappedActors;

	bool IsTarget(ESOWCharacterType TargetType);
};
