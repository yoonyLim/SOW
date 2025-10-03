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

	ProjectileFxComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile FX"));
	ProjectileFxComp->SetupAttachment(GetRootComponent());
}

void AProjectileBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AProjectileBase::ActivateMovement()
{
	// 투사체의 움직임과 이펙트, 콜리션을 모두 활성화합니다.

	if (ProjectileHitCollisionComp)
	{
		ProjectileHitCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	ProjectileMoveComp->Activate();
	ProjectileFxComp->Activate();
}

void AProjectileBase::ResetProjectile()
{
	// 투사체의 모든 속성을 초기화하고, 인게임에서의 역할을 비활성화합니다.

	OverlappedActors.Empty();
	TargetActor = nullptr;
	bHitDone = false;
	InGame = false;

	// 콜리전 초기화
	if (ProjectileHitCollisionComp)
	{
		ProjectileHitCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	ProjectileMoveComp->Deactivate();
	ProjectileFxComp->Deactivate();

	SetProjectileInGame(false);
	SetActorHiddenInGame(true);
}

void AProjectileBase::BP_DestroyProjectile()
{
	// 블루프린트 내에서 구현할 수 있는 투사체 파괴 함수입니다.
	// 현재 TurretProjectileBase에서 활용하고 있습니다.

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

void AProjectileBase::InitProjectileProperties(FTransform InTransform, ETurretTargetSelectionPolicy InPolicy, FGameplayEffectSpecHandle InHandle, bool InMovement, float InSpeed, float InDuration, float InScale, AActor* InTargetActor)
{
	// 투사체의 속성을 초기화합니다.
	// 투사체의 데이터는 TurretPropertyData 데이터테이블에 일괄적으로 정의되고 있습니다.

	OwnerPolicy = InPolicy;
	OwnerDamageEffectSpecHandle = InHandle;
	TargetActor = InTargetActor;

	if (OriginScaledCollisionExtent == FVector::ZeroVector) {
		OriginScaledCollisionExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent();
	}

	ProjectileHitCollisionComp->SetBoxExtent(OriginScaledCollisionExtent * InScale);

	ProjectileMoveComp->InitialSpeed = InSpeed;
	ProjectileMoveComp->MaxSpeed = InSpeed;
	ProjectileMoveComp->Velocity = InTransform.GetRotation().GetForwardVector() * ProjectileMoveComp->InitialSpeed;

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
	// 투사체의 인게임 상태를 정의합니다.
	// false인 경우 투사체는 인게임에서 어떤 영향도 주지 못하는 상태로 가정합니다.

	InGame = In;
}
void AProjectileBase::SendTargetDeadEventToInstigator(AActor* InCheckingTarget)
{
	// 투사체의 Instigator에 충돌한 타겟이 사망했다는 정보를 전달합니다.
	// 만약, 공격 대상이 사망한 이력을 확인하여 후속 조치가 필요한 경우 사용할 수 있습니다.
	// 대상 사망 시, Shared.Event.TargetDead 이벤트를 전달하므로, Wait 또는 Triggered Ability를 활용할 수 있습니다.

	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(InCheckingTarget, SOWGameplayTags::Shared_Status_Dead)) {
		ASOWCharacter* SOWInstigator = Cast<ASOWCharacter>(GetInstigator());

		FGameplayEventData Data;
		Data.Target = InCheckingTarget;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			SOWInstigator,
			SOWGameplayTags::Shared_Event_TargetDead,
			Data
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
