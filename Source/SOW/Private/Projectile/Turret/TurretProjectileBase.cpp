// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Turret/TurretProjectileBase.h"
#include "Components/SOWProjectilePoolingComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWBlueprintFunctionLibrary.h"

// Sets default values
ATurretProjectileBase::ATurretProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMoveComp->ProjectileGravityScale = 0.f;
}




void ATurretProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	CachedInstigator = CastChecked<ASOWCharacterTurretBase>(GetInstigator());
}

void ATurretProjectileBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!GetProjectileInGame()) return;

	if (HasMovement) {
		if (CheckOutOfRange()) {
			BP_DestroyProjectile();
		}

		if (bHitOnce && (!TargetActor || (Cast<ASOWCharacter>(TargetActor) && USOWBlueprintFunctionLibrary::DoesActorHasTag(TargetActor, SOWGameplayTags::Shared_Status_Dead)))) {
			BP_DestroyProjectile();
		}
		else { 
			FaceToTargetActor(); 
		}
		
	}
}

void ATurretProjectileBase::FaceToTargetActor() {
	if (!HasMovement) return;

	if (!TargetActor) return;

	if (bHitDone) return;

	bool bValidTarget = TargetActor->Implements<USOWCharacterTypeInterface>();
	if (!bValidTarget) return;

	bool bTargetAlive = !USOWBlueprintFunctionLibrary::DoesActorHasTag(TargetActor, SOWGameplayTags::Shared_Status_Dead);

	if (bTargetAlive) {
		FVector DirVector = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		ProjectileMoveComp->Velocity = DirVector * ProjectileMoveComp->InitialSpeed;
		SetActorRotation(DirVector.Rotation());
	}
}

bool ATurretProjectileBase::CheckOutOfRange()
{
	float range = CachedInstigator->GetDetectionRangeRadius();
	float criticValue = (range + 0.5f) * 116.f; // Tile Size : 116 -> Hard Coding / 한 변의 길이
	
	FVector VLeft = FVector::LeftVector;
	FVector VForward = FVector::ForwardVector;

	FVector PosVector = GetActorLocation() - CachedInstigator->GetActorLocation();

	float LeftProj = FMath::Abs(FVector::DotProduct(VLeft, PosVector));
	float ForwardProj = FMath::Abs(FVector::DotProduct(VForward, PosVector));

	return (LeftProj >= criticValue) || (ForwardProj >= criticValue);

}

void ATurretProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlapping Callback Function
	Super::OnCollisionHit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!CachedInstigator.Get()) return;

	if (!OtherActor) return;

	if (!Cast<ASOWCharacter>(OtherActor) || Cast<ASOWCharacterPlayer>(OtherActor)) return;

	if (CachedInstigator.Get() == OtherActor || OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.AddUnique(OtherActor);

	// Apply Damage or Process After Effect like Gradual reinforcement
	bHitDone = true;
	BP_PostProjectileHit(OtherActor);
}

void ATurretProjectileBase::OnCollisionOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionOut(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (!OtherActor) return;

	BP_PostProjectileOut(OtherActor);
}

bool ATurretProjectileBase::IsHostileTarget(AActor* Target)
{
	if (!Target || !Target->Implements<USOWCharacterTypeInterface>()) return false;

	ESOWCharacterType TargetType = Cast<ISOWCharacterTypeInterface>(Target)->GetSOWCharacterType();
	return USOWBlueprintFunctionLibrary::IsTarget(OwnerPolicy, TargetType);
}


void ATurretProjectileBase::BP_DestroyProjectile()
{
	//checkf(CachedInstigator.Get()->GetProjectilePoolingComponent(), TEXT("No Pool Found For %s"), *CachedInstigator.Get()->GetActorNameOrLabel());

	if (!CachedInstigator.Get() || !CachedInstigator.Get()->GetProjectilePoolingComponent()) Destroy();

	CachedInstigator.Get()->GetProjectilePoolingComponent()->ReturnProjectile(this);
	UE_LOG(LogTemp, Warning, TEXT("Return to Pool : %s"), *this->GetActorNameOrLabel());
}