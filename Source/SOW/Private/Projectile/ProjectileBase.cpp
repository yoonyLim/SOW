// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
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
	FVector ScaledExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * ScaleRatio;
	ProjectileHitCollisionComp->SetBoxExtent(ScaledExtent);
	//ProjectileMoveComp->SetVelocityInLocalSpace(GetActorForwardVector() * Speed);
	ProjectileMoveComp->InitialSpeed = Speed;
	ProjectileMoveComp->MaxSpeed = Speed;
	ProjectileMoveComp->Velocity = GetActorForwardVector() * Speed;
}

FGameplayEffectSpecHandle AProjectileBase::GetDamageSpecHandle() const
{
	return OwnerDamageEffectSpecHandle;
}

void AProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Hit Detected"));
}