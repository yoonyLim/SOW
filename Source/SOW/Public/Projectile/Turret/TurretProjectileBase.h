// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOWEnumTypes.h"
#include "GameplayEffectTypes.h"
#include "TurretProjectileBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UProjectileMovementComponent;

class ASOWCharacterTurretBase;

enum class EProjectileType :uint8 {
	Normal,
	Explosive,
	Piercing,
	Summonning
};

UCLASS()
class SOW_API ATurretProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretProjectileBase();

	
	UFUNCTION(BlueprintImplementableEvent, Category = "Turret|Hit", meta = (DisplayName = "Post Projectile Hit"))
	void BP_PostProjectileHit(AActor* Target);								// Process Atfer Effect When Detecting Hit

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hit Callback
	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma region SetOnConstructor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Component")
	UStaticMeshComponent* ProjectileMeshComp;					// Visible mesh, no collision

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Component")
	UBoxComponent* ProjectileHitCollisionComp;					// Real collision detection area

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Component")
	UProjectileMovementComponent* ProjectileMoveComp;			// Dealing with the movement of projectile
#pragma endregion

	
#pragma region SetBeforeSpawning
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Combat", meta = (ExposeOnSpawn = true))
	ETurretTargetSelectionPolicy OwnerPolicy;					// What the turret should target

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Combat", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle OwnerDamageEffectSpecHandle;		// Apply the owner turret's damage specs

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Properties", meta = (ExposeOnSpawn = true))
	bool HasMovement;											// if projectile is movable, it must be true

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Properties", meta = (ExposeOnSpawn = true, EditCondition = "HasMovement", EditConditionHides))
	float Speed;												// Projectile Movement Speed. 																			// Actors can stay in the scene for this amount of time

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Properties", meta = (ExposeOnSpawn = true))
	float Duration;												// if projetile has movement, speed and range determine the duration. if not, must be assigned. 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Properties", meta = (ExposeOnSpawn = true))
	float ScaleRatio = 1.f;										// Collision Scale
#pragma endregion

private:

#pragma region InternalProperties
	TWeakObjectPtr<ASOWCharacterTurretBase> CachedInstigator;	// if projectile needs to check instigator several time, it must call GetInstigator() too.

	TArray<AActor*> OverlappedActors;							// Once a target has been damaged, it must be ignored.

	FVector TargetLocation;										// If you need to specify a location other than the target
#pragma endregion


public:

#pragma region Getter
	UFUNCTION(BlueprintPure, Category = "Projectile|Combat")
	FGameplayEffectSpecHandle GetDamageSpecHandle() const;
#pragma endregion

	
};


/* About void BP_PostProjectileHit();
	if normal projectile, just apply damage to single target and destroy immediately
	else if explosive, In addition to damaging the target, it also applies a certain percentage of damage to surrounding targets.
	else if piercing, Does not destroy target even if it deals damage. 
	else if summonning, just apply effect in living duration

	it can be customized.
*/