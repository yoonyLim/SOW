// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWEnumTypes.h"
#include "SOWProjectilePoolingComponent.generated.h"

class AProjectileBase;
class ASOWCharacter;
struct FGameplayEffectSpecHandle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWProjectilePoolingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWProjectilePoolingComponent();

	UFUNCTION(BlueprintCallable)
	void CreateAndFixPool(TSubclassOf<AProjectileBase> ProjectileClass, int32 NewPoolNumber , int32 PoolSize);

	UFUNCTION(BlueprintCallable)
	void RemovePool(int32 PoolNumber);

	UFUNCTION(BlueprintCallable)
	AProjectileBase* SpawnProjectile(const FTransform& SpawnTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale, ASOWCharacter* InTargetActor, int32 PoolNumber);

	UFUNCTION(BlueprintCallable)
	void ReturnProjectile(AProjectileBase* Projectile);

private:
	int PoolNumber;
	TMap<int, TArray<AProjectileBase*>> Pools;
	TMap<int, TSubclassOf<AProjectileBase>> PoolClasses;
		
};
