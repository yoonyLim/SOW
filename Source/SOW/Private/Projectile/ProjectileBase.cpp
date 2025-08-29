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
	ProjectileMoveComp->bRotationFollowsVelocity = true;
	//ProjectileMoveComp->bIsHomingProjectile = true;

	ProjectileFxComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile FX"));
	ProjectileFxComp->SetupAttachment(GetRootComponent());
}

void AProjectileBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

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
	TargetActor = nullptr;

	if (ProjectileMoveComp)
	{
		//ProjectileMoveComp->StopMovementImmediately();
		//ProjectileMoveComp->Velocity = FVector::ZeroVector;
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

void AProjectileBase::InitProjectileProperties(FTransform InTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale, ASOWCharacter* InTargetActor)
{
	OwnerPolicy = InPolicy;
	OwnerDamageEffectSpecHandle = InHandle;
	TargetActor = InTargetActor;

	

	if (OriginScaledCollisionExtent == FVector::ZeroVector) {
		OriginScaledCollisionExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent();
	}

	//FVector ScaledCollisionExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * InScale;
	ProjectileHitCollisionComp->SetBoxExtent(OriginScaledCollisionExtent * InScale);

	ProjectileMoveComp->InitialSpeed = InSpeed;
	ProjectileMoveComp->MaxSpeed = InSpeed;
	ProjectileMoveComp->Velocity = InTransform.GetRotation().GetForwardVector() * ProjectileMoveComp->InitialSpeed;

	//ProjectileMoveComp->bIsHomingProjectile = true;
	//ProjectileMoveComp->HomingTargetComponent = TargetActor->GetRootComponent();
	ProjectileMoveComp->HomingAccelerationMagnitude = 100.f;

	if (OriginScaledMeshExtent == FVector::ZeroVector) {
		OriginScaledMeshExtent = ProjectileMeshComp->GetRelativeScale3D();
	}
	ProjectileMeshComp->SetWorldScale3D(FVector(OriginScaledMeshExtent.X, OriginScaledMeshExtent.Y, OriginScaledMeshExtent.Z) * InScale);

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
