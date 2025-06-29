// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Characters/SOWCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "SOWGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileHitCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Projectile Hit Collision"));
	SetRootComponent(ProjectileHitCollisionComp);
	ProjectileHitCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCollisionHit);

	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMeshComp->SetupAttachment(GetRootComponent());

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	FVector ScaledCollisionExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * ScaleRatio;
	ProjectileHitCollisionComp->SetBoxExtent(ScaledCollisionExtent);
	//ProjectileMoveComp->SetVelocityInLocalSpace(GetActorForwardVector() * Speed);
	ProjectileMoveComp->InitialSpeed = Speed;
	ProjectileMoveComp->MaxSpeed = Speed;
	ProjectileMoveComp->Velocity = GetActorForwardVector() * Speed;

	FVector ScaledMeshExtent = ProjectileMeshComp->GetRelativeScale3D() * ScaleRatio;
	ProjectileMeshComp->SetWorldScale3D(FVector(ScaledMeshExtent.X, ScaledMeshExtent.Y, ScaledMeshExtent.Z));
}

FGameplayEffectSpecHandle AProjectileBase::GetDamageSpecHandle() const
{
	return OwnerDamageEffectSpecHandle;
}

void AProjectileBase::SendTargetDeadEventToInstigator(AActor* InActor)
{
	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(InActor, SOWGameplayTags::Shared_Status_Dead)) {
		ASOWCharacter* SOWInstigator = Cast<ASOWCharacter>(GetInstigator());
		SOWInstigator->OnTargetDead.Broadcast(InActor);
	}
}

void AProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//UE_LOG(LogTemp, Warning, TEXT("Hit Detected"));
}