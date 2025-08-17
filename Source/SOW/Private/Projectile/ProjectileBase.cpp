// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Characters/SOWCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "SOWGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileHitCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Projectile Hit Collision"));
	SetRootComponent(ProjectileHitCollisionComp);
	ProjectileHitCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCollisionHit);
	ProjectileHitCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AProjectileBase::OnCollisionOut);

	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMeshComp->SetupAttachment(GetRootComponent());

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileFxComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile FX"));
	ProjectileFxComp->SetupAttachment(GetRootComponent());
}

//void AProjectileBase::DeactivateMovement()
//{
//	ProjectileMoveComp->Deactivate();
//	ProjectileFxComp->Deactivate();
//}

void AProjectileBase::ActivateMovement()
{

	if (ProjectileHitCollisionComp)
	{
		ProjectileHitCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	ProjectileMoveComp->Activate();
	ProjectileFxComp->Activate();
}

void AProjectileBase::ResetProjectile()
{
	OverlappedActors.Empty();

	if (ProjectileMoveComp)
	{
		ProjectileMoveComp->StopMovementImmediately();
		ProjectileMoveComp->Velocity = FVector::ZeroVector;
	}

	// 콜리전 초기화
	if (ProjectileHitCollisionComp)
	{
		ProjectileHitCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ProjectileHitCollisionComp->IgnoreActorWhenMoving(nullptr, false);
	}
	ProjectileMoveComp->Deactivate();
	ProjectileFxComp->Deactivate();

	SetProjectileInGame(false);
	SetActorHiddenInGame(true);
}

void AProjectileBase::BP_DestroyProjectile()
{
	Destroy();
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	FVector ScaledCollisionExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * ScaleRatio;
	ProjectileHitCollisionComp->SetBoxExtent(ScaledCollisionExtent);
	//ProjectileMoveComp->SetVelocityInLocalSpace(GetActorForwardVector() * Speed);
	ProjectileMoveComp->InitialSpeed = Speed;
	ProjectileMoveComp->MaxSpeed = Speed;
	ProjectileMoveComp->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMoveComp->InitialSpeed);

	FVector ScaledMeshExtent = ProjectileMeshComp->GetRelativeScale3D() * ScaleRatio;
	ProjectileMeshComp->SetWorldScale3D(FVector(ScaledMeshExtent.X, ScaledMeshExtent.Y, ScaledMeshExtent.Z));

	//ProjectileMoveComp->bRotationFollowsVelocity = true;
}

FGameplayEffectSpecHandle AProjectileBase::GetDamageSpecHandle() const
{
	return OwnerDamageEffectSpecHandle;
}

int AProjectileBase::GetPoolNumber() const
{
	return PoolNumber;
}

bool AProjectileBase::GetProjectileInGame() const
{
	return InGame;
}

void AProjectileBase::InitProjectileProperties(FTransform InTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale)
{
	OwnerPolicy = InPolicy;
	OwnerDamageEffectSpecHandle = InHandle;
	
	FVector ScaledCollisionExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * InScale;
	ProjectileHitCollisionComp->SetBoxExtent(ScaledCollisionExtent);

	ProjectileMoveComp->InitialSpeed = InSpeed;
	ProjectileMoveComp->MaxSpeed = InSpeed;
	ProjectileMoveComp->Velocity = InTransform.GetRotation().GetForwardVector() * ProjectileMoveComp->InitialSpeed;

	FVector ScaledMeshExtent = ProjectileMeshComp->GetRelativeScale3D() * InScale;
	ProjectileMeshComp->SetWorldScale3D(FVector(ScaledMeshExtent.X, ScaledMeshExtent.Y, ScaledMeshExtent.Z));

	HasMovement = InMovement;
	Duration = InDuration;
}

void AProjectileBase::SetPoolNumber(int InNumber)
{
	PoolNumber = InNumber;
}
void AProjectileBase::SetProjectileInGame(bool In)
{
	InGame = In;
}
void AProjectileBase::SendTargetDeadEventToInstigator(AActor* InCheckingTarget)
{
	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(InCheckingTarget, SOWGameplayTags::Shared_Status_Dead)) {
		ASOWCharacter* SOWInstigator = Cast<ASOWCharacter>(GetInstigator());

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			SOWInstigator,
			SOWGameplayTags::Shared_Event_TargetDead,
			FGameplayEventData()
		);

		UE_LOG(LogTemp, Warning, TEXT("Sended Target Dead Event"));
		//SOWInstigator->OnTargetDead.Broadcast(InActor);
	}
}

void AProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//UE_LOG(LogTemp, Warning, TEXT("Hit Detected"));
}

void AProjectileBase::OnCollisionOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit Detected"));
}
