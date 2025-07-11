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

	ATurretMeleeHitCollision();

	void ToggleCollision(bool bShouldEnable);							// Set Collsion based on ANS beginning or ending

	void ApplyDamageToAllHitTargets();
	void ApplyDamageToFirstHitTargets();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Combat")
	UBoxComponent* MeleeHitCollision;									// Hit Detection Component

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Combat", meta = (ExposeOnSpawn = true))
	ETurretTargetSelectionPolicy OwnerPolicy;							// What the turret should target
			
private:

	UFUNCTION()
	void MeleeHit														// Callback Function to handle enemies that collide with the collision
	(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TWeakObjectPtr<ASOWCharacterTurretBase> CachedInstigator;			// if projectile needs to check instigator several time, it must call GetInstigator() too.

	TArray<AActor*> OverlappedActors;									// Once a target has been damaged, it must be ignored.

	bool bHasSplashApplied;
};
