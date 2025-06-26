// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/ProjectileBase.h"
#include "EnemyProjectileBase.generated.h"

class ASOWCharacterEnemyBase;

/**
 * 
 */
UCLASS()
class SOW_API AEnemyProjectileBase : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AEnemyProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hit Callback
	virtual void OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

#pragma region InternalProperties
	TWeakObjectPtr<ASOWCharacterEnemyBase> CachedInstigator;	// if projectile needs to check instigator several time, it must call GetInstigator() too.
#pragma endregion
};
