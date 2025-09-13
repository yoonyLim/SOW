// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile/ProjectileBase.h"
#include "TurretProjectileBase.generated.h"

class ASOWCharacterTurretBase;
class ASOWCharacter;

UCLASS()
class SOW_API ATurretProjectileBase : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretProjectileBase();
	virtual void BP_DestroyProjectile() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Hit Callback
	virtual void OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnCollisionOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	bool IsHostileTarget(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetHitDone(bool bHit);

	void FaceToTargetActor();
	
private:
	bool CheckOutOfRange();
	

#pragma region InternalProperties
	TWeakObjectPtr<ASOWCharacterTurretBase> CachedInstigator;	// if projectile needs to check instigator several time, it must call GetInstigator() too.
#pragma endregion

};


/* About void BP_PostProjectileHit();
	if normal projectile, just apply damage to single target and destroy immediately
	else if explosive, In addition to damaging the target, it also applies a certain percentage of damage to surrounding targets.
	else if piercing, Does not destroy target even if it deals damage. 
	else if summonning, just apply effect in living duration

	it can be customized.
*/