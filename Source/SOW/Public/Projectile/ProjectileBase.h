// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOWEnumTypes.h"
#include "GameplayEffectTypes.h"
#include "ProjectileBase.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class SOW_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AProjectileBase();


	UFUNCTION(BlueprintImplementableEvent, Category = "Turret|Hit", meta = (DisplayName = "Post Projectile Hit"))
	void BP_PostProjectileHit(AActor* Target);								// Process Atfer Effect When Detecting Hit

	UFUNCTION(BlueprintImplementableEvent, Category = "Turret|Hit", meta = (DisplayName = "Post Projectile Out"))
	void BP_PostProjectileOut(AActor* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hit Callback
	UFUNCTION()
	virtual void OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCollisionOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
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


#pragma region InternalProperties
	TArray<AActor*> OverlappedActors;							// Once a target has been damaged, it must be ignored.

	FVector TargetLocation;										// If you need to specify a location other than the target
#pragma endregion


public:

#pragma region Getter
	UFUNCTION(BlueprintPure, Category = "Projectile|Combat")
	FGameplayEffectSpecHandle GetDamageSpecHandle() const;
#pragma endregion


	UFUNCTION(BlueprintCallable)
	void SendTargetDeadEventToInstigator(AActor* InActor);
};
