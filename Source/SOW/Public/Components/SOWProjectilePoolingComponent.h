// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWEnumTypes.h"
#include "SOWProjectilePoolingComponent.generated.h"

class AProjectileBase;
struct FGameplayEffectSpecHandle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWProjectilePoolingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWProjectilePoolingComponent();

	UFUNCTION(BlueprintCallable)
	void CreateAndFixPool(TSubclassOf<AProjectileBase> ProjectileClass, int32 PoolSize);

	UFUNCTION(BlueprintCallable)
	AProjectileBase* SpawnProjectile(const FTransform& SpawnTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale);

	UFUNCTION(BlueprintCallable)
	void ReturnProjectile(AProjectileBase* Projectile);

private:
	int PoolNumber;
	TMap<int, TArray<AProjectileBase*>> Pools;
	TMap<int, TSubclassOf<AProjectileBase>> PoolClasses;
		
};
