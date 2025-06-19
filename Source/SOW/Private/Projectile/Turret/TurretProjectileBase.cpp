// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Turret/TurretProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Interface/SOWCharacterTypeInterface.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "SOWGameplayTags.h"

// Sets default values
ATurretProjectileBase::ATurretProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileHitCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Projectile Hit Collision"));
	SetRootComponent(ProjectileHitCollisionComp);

	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMeshComp->SetupAttachment(GetRootComponent());

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	
}

void ATurretProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	ProjectileHitCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATurretProjectileBase::OnCollisionHit);
	FVector ScaledExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * ScaleRatio;
	ProjectileHitCollisionComp->SetBoxExtent(ScaledExtent);
	//ProjectileMoveComp->SetVelocityInLocalSpace(GetActorForwardVector() * Speed);
	ProjectileMoveComp->InitialSpeed = Speed;
	ProjectileMoveComp->MaxSpeed = Speed;
	ProjectileMoveComp->Velocity = GetActorForwardVector() * Speed;
	// Terminated Time is needed to set with Duration
}

void ATurretProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlapping Callback Function

	if (!CachedInstigator.Get()) {
		if (!GetInstigator()) return;


		CachedInstigator = CastChecked<ASOWCharacterTurretBase>(GetInstigator());
		if (!CachedInstigator.Get()) {
			return;
		}
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

FGameplayEffectSpecHandle ATurretProjectileBase::GetDamageSpecHandle() const
{
	return OwnerDamageEffectSpecHandle;
}
