// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWProjectilePoolingComponent.h"
#include "GameplayEffectTypes.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "Projectile/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "SOWGameplayTags.h"




USOWProjectilePoolingComponent::USOWProjectilePoolingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PoolNumber = -1;
	// ...
}

void USOWProjectilePoolingComponent::CreateAndFixPool(TSubclassOf<AProjectileBase> ProjectileClass, int32 NewPoolNumber ,int32 PoolSize = 5)
{
	// if you want create new pool, just use this function with new projectile subclass

	PoolClasses.Add(NewPoolNumber, ProjectileClass);

	TArray<AProjectileBase*>& Pool = Pools.FindOrAdd(NewPoolNumber);
	for (int32 i = 0; i < PoolSize; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner(); // 소유자 지정
		SpawnParams.Instigator = Cast<APawn>(GetOwner()); // 현재 액터의 Instigator 사용 (또는 직접 지정)

		AProjectileBase* NewProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		NewProjectile->ResetProjectile();

		Pool.Add(NewProjectile);
	}
}

void USOWProjectilePoolingComponent::RemovePool(int32 NewPoolNumber)
{
	Pools.Remove(NewPoolNumber);
	PoolClasses.Remove(NewPoolNumber);
}



AProjectileBase* USOWProjectilePoolingComponent::SpawnProjectile(const FTransform& SpawnTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale, AActor* InTargetActor, int32 NewPoolNumber)
{
	// if you want to spawn projectile from pool, just use this function. 
	// you need to assign other properties from owner turret or enemy

	TArray<AProjectileBase*>* Pool = Pools.Find(NewPoolNumber);

	if (Pool && Pool->Num() <= 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner(); // 소유자 지정
		SpawnParams.Instigator = Cast<APawn>(GetOwner()); // 현재 액터의 Instigator 사용 (또는 직접 지정)

		AProjectileBase* NewProjectile = GetWorld()->SpawnActor<AProjectileBase>(PoolClasses[NewPoolNumber], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		NewProjectile->ResetProjectile();

		Pool->Add(NewProjectile);
	}
	

	if (!InTargetActor) return nullptr;

	AProjectileBase* Projectile = Pool->Pop();
	Projectile->InitProjectileProperties(SpawnTransform, InPolicy, InHandle, InMovement, InSpeed, InDuration, InScale, InTargetActor);
	Projectile->SetPoolNumber(NewPoolNumber);

	Projectile->SetActorTransform(SpawnTransform);
	Projectile->SetProjectileInGame(true);
	Projectile->SetActorHiddenInGame(false);
	Projectile->ActivateMovement();

	Projectile->BP_ExecuteProjectileSpawnEffect();
	return Projectile;
}

void USOWProjectilePoolingComponent::ReturnProjectile(AProjectileBase* Projectile)
{
	// when a projectile has to be destroyed, use this function to return to pool the projectile 

	Projectile->ResetProjectile();
	Projectile->SetActorTransform(FTransform::Identity);
	
	if (!Pools.Find(Projectile->GetPoolNumber())) {
		Projectile->Destroy();
		return;
	}
	Pools[Projectile->GetPoolNumber()].Add(Projectile);
}


