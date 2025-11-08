// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Turret/TurretProjectileBase.h"
#include "Components/SOWProjectilePoolingComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SOWTurretCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWBlueprintFunctionLibrary.h"

#include "SOWGameInstance.h"

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

	if(!IsValid(CachedInstigator.Get())) Destroy();

	if (HasMovement) {
		if (CheckOutOfRange()) {
			BP_DestroyProjectile();
		}
		FaceToTargetActor();
	}
}

void ATurretProjectileBase::FaceToTargetActor() {
	if (!HasMovement) return;

	if (!TargetActor) return;

	//if (bHitDone || !IsValid(TargetActor) || TargetActor->IsPendingKillPending()) {
	//	
	//	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ZPosWhenTargetInvalid));
	//	ProjectileMoveComp->Velocity = FVector(ProjectileMoveComp->Velocity.X, ProjectileMoveComp->Velocity.Y, 0.f);

	//	TargetActor = nullptr;
	//	return;
	//} 

	//if (USOWBlueprintFunctionLibrary::DoesActorHasTag(TargetActor, SOWGameplayTags::Shared_Status_Dead)) {
	//	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ZPosWhenTargetInvalid));
	//	ProjectileMoveComp->Velocity = FVector(ProjectileMoveComp->Velocity.X, ProjectileMoveComp->Velocity.Y, 0.f);

	//	TargetActor = nullptr;
	//	return;
	//}

	//if (!TargetActor) return;

	//bool bValidTarget = TargetActor->Implements<USOWCharacterTypeInterface>();
	//if (!bValidTarget) return;

	////bool bTargetAlive = !USOWBlueprintFunctionLibrary::DoesActorHasTag(TargetActor, SOWGameplayTags::Shared_Status_Dead);

	//FVector DirVector = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	//ProjectileMoveComp->Velocity = DirVector * ProjectileMoveComp->InitialSpeed;
	//SetActorRotation(DirVector.Rotation());

	if (bHitDone || !IsValid(TargetActor) || TargetActor->IsPendingKillPending() ||
		USOWBlueprintFunctionLibrary::DoesActorHasTag(TargetActor, SOWGameplayTags::Shared_Status_Dead))
	{
		FVector CurrentLoc = GetActorLocation();

		// Z 위치 고정
		SetActorLocation(FVector(CurrentLoc.X, CurrentLoc.Y, ZPosWhenTargetInvalid));

		// 이전 속도를 기반으로 수평 벡터만 유지
		FVector PrevVelocity = ProjectileMoveComp->Velocity;
		PrevVelocity.Z = 0.f; // Z 하강 방지
		ProjectileMoveComp->Velocity = PrevVelocity.GetSafeNormal() * ProjectileMoveComp->InitialSpeed;

		// 마지막 회전 방향도 수평으로 보정
		FRotator FlatRot = PrevVelocity.GetSafeNormal2D().Rotation();
		SetActorRotation(FlatRot);

		// 더 이상 타겟 추적 안 함
		TargetActor = nullptr;
		return;
	}


	FVector DirVector = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	ProjectileMoveComp->Velocity = DirVector * ProjectileMoveComp->InitialSpeed;
	SetActorRotation(DirVector.Rotation());
}

void ATurretProjectileBase::InitProjectileProperties(FTransform InTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale, AActor* InTargetActor)
{
	Super::InitProjectileProperties(InTransform, InPolicy, InHandle, InMovement, InSpeed, InDuration, InScale, InTargetActor);

	ZPosWhenTargetInvalid = InTargetActor->GetActorLocation().Z + 50.f;
}

bool ATurretProjectileBase::CheckOutOfRange()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetGameInstance());
	if (!GI || !IsValid(CachedInstigator.Get())) return false;

	float range = CachedInstigator->GetDetectionRangeRadius();
	float criticValue = (range + 0.5f) * GI->GetWorldTileSize(); // Tile Size : 116 -> Hard Coding / 한 변의 길이
	
	FVector VLeft = FVector::LeftVector;
	FVector VForward = FVector::ForwardVector;

	FVector PosVector = GetActorLocation() - CachedInstigator->GetActorLocation();

	float LeftProj = FMath::Abs(FVector::DotProduct(VLeft, PosVector));
	float ForwardProj = FMath::Abs(FVector::DotProduct(VForward, PosVector));
	float HeightProj = FMath::Abs(FVector::DotProduct(FVector(0,0,1.f), PosVector));

	return (LeftProj >= criticValue) || (ForwardProj >= criticValue) || (HeightProj >= criticValue);

}

void ATurretProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlapping Callback Function
	Super::OnCollisionHit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!IsValid(CachedInstigator.Get())) return;

	if (!IsValid(OtherActor)) return;

	if (!Cast<ASOWCharacter>(OtherActor) || Cast<ASOWCharacterPlayer>(OtherActor)) return;

	if (CachedInstigator.Get() == OtherActor || OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.AddUnique(OtherActor);

	// Apply Damage or Process After Effect like Gradual reinforcement
	//bHitDone = true;
	BP_PostProjectileHit(OtherActor);
}

void ATurretProjectileBase::OnCollisionOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCollisionOut(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (!IsValid(OtherActor)) return;

	BP_PostProjectileOut(OtherActor);
}

bool ATurretProjectileBase::IsHostileTarget(AActor* Target)
{
	if (!IsValid(Target) || !Target->Implements<USOWCharacterTypeInterface>()) return false;

	ESOWCharacterType TargetType = Cast<ISOWCharacterTypeInterface>(Target)->GetSOWCharacterType();
	return USOWBlueprintFunctionLibrary::IsTarget(OwnerPolicy, TargetType);
}

void ATurretProjectileBase::SetHitDone(bool bHit)
{
	bHitDone = bHit;
}

void ATurretProjectileBase::ClearHitActors()
{
	OverlappedActors.Empty();
}


void ATurretProjectileBase::BP_DestroyProjectile(bool AttackSucceed)
{
	//checkf(CachedInstigator.Get()->GetProjectilePoolingComponent(), TEXT("No Pool Found For %s"), *CachedInstigator.Get()->GetActorNameOrLabel());

	BP_PostProjectileDestroyed(AttackSucceed);

	if (!IsValid(CachedInstigator.Get()) || !CachedInstigator.Get()->GetProjectilePoolingComponent()) {
		Destroy();
		return;
	} 

	if (AttackSucceed) 
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedInstigator.Get(), SOWGameplayTags::Turret_Event_Attack_Done, FGameplayEventData());
	}
		
	
	CachedInstigator.Get()->GetProjectilePoolingComponent()->ReturnProjectile(this);
	//UE_LOG(LogTemp, Warning, TEXT("Return to Pool : %s"), *this->GetActorNameOrLabel());
}