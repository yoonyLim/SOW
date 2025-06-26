// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Turret/TurretProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWBlueprintFunctionLibrary.h"

// Sets default values
ATurretProjectileBase::ATurretProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATurretProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATurretProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlapping Callback Function
	Super::OnCollisionHit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!CachedInstigator.Get()) {
		CachedInstigator = CastChecked<ASOWCharacterTurretBase>(GetInstigator());

		if (!CachedInstigator.Get()) return;
	}

	if (!OtherActor) return;

	if (CachedInstigator.Get() == OtherActor || OverlappedActors.Contains(OtherActor)) return;

	if (!OtherActor->Implements<USOWCharacterTypeInterface>()) return;

	ESOWCharacterType TargetType = Cast<ISOWCharacterTypeInterface>(OtherActor)->GetSOWCharacterType();
	if (!USOWBlueprintFunctionLibrary::IsTarget(OwnerPolicy, TargetType)) return;

	OverlappedActors.AddUnique(OtherActor);

	// Apply Damage or Process After Effect like Gradual reinforcement
	BP_PostProjectileHit(OtherActor);
}


