// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Enemy/EnemyProjectileBase.h"
#include "Structures/Enemies/EnemyStructs.h"
#include "Engine/DataTable.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h" 
#include "GameplayEffectTypes.h"  


AEnemyProjectileBase::AEnemyProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyProjectileBase::BeginPlay()
{
    Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[RangedAttack-Projectile] Projectile generated")); // LogAdded

    // 1) DataTable과 RowName 유효성 체크
    if (!EnemyAttributesTable || EnemyTypeRowName.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] Missing ProjectileAttributesTable or ProjectileTypeRowName"), *GetName());
        return;
    }

    // 2) DataTable에서 Row 읽기
    static const FString Ctxt(TEXT("InitProjectile"));
	const FProjectileAttributeData* Row = EnemyAttributesTable
        ->FindRow<FProjectileAttributeData>(EnemyTypeRowName, Ctxt);
    if (!Row)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] No row '%s' in %s"),
            *GetName(), *EnemyTypeRowName.ToString(), *EnemyAttributesTable->GetName());
        return;
    }

    // 3) 할당
    HasMovement = Row->bHasMovement;
    Speed = Row->Speed;
    Duration = Row->Duration;
    ScaleRatio = Row->ScaleRatio;

	UE_LOG(LogTemp, Warning, TEXT("[RangedAttack-Projectile] HasMovement: %s"),
		HasMovement ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("[RangedAttack-Projectile] Speed: %.2f"), Speed);
	UE_LOG(LogTemp, Warning, TEXT("[RangedAttack-Projectile] Duration: %.2f"), Duration);
	UE_LOG(LogTemp, Warning, TEXT("[RangedAttack-Projectile] ScaleRatio: %.2f"), ScaleRatio);

    // 4) 데미지 이펙트 클래스 & 레벨
    DamageEffectClass = Row->DamageEffectClass;
    EffectLevel = Row->EffectLevel;

	// ======================컴포넌트 재 초기화====================================================================
	if (HasMovement && ProjectileMoveComp)
	{
		ProjectileMoveComp->InitialSpeed = Speed;
		ProjectileMoveComp->MaxSpeed = Speed;
		ProjectileMoveComp->Velocity = GetActorForwardVector() * Speed;
	}

	// 지속시간 재설정
	SetLifeSpan(Duration);

	// 충돌 크기 재적용
	if (ProjectileHitCollisionComp)
	{
		FVector NewExtent = ProjectileHitCollisionComp->GetUnscaledBoxExtent() * ScaleRatio;
		ProjectileHitCollisionComp->SetBoxExtent(NewExtent);
	}

	// 메시 스케일 재적용
	if (ProjectileMeshComp)
	{
		ProjectileMeshComp->SetWorldScale3D(
			ProjectileMeshComp->GetRelativeScale3D() * ScaleRatio
		);
	}
}

void AEnemyProjectileBase::OnCollisionHit(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	//Super::OnCollisionHit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!CachedInstigator.Get()) {
		CachedInstigator = CastChecked<ASOWCharacterEnemyBase>(GetInstigator());
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] CachedInstigator set: %s"),
			*GetNameSafe(CachedInstigator.Get()));  // LogAdded

		if (!CachedInstigator.Get()) {
			UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] No valid instigator, abort"));  // LogAdded
			return;
		}
	}

	//null, instigator -> 무시
	if (!OtherActor || OtherActor == CachedInstigator.Get()) {
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] Ignored actor: %s"),
			OtherActor ? *OtherActor->GetName() : TEXT("None"));  // LogAdded
		return;
	}


	//중복시 무시
	if (OverlappedActors.Contains(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] Already hit actor, skipping: %s"),
			*OtherActor->GetName());  // LogAdded
		return;
	}

	//중복 방지 리스트에 추가
	OverlappedActors.AddUnique(OtherActor);


	//========스펙 핸들 생성 및 부모 멤버에 할당==========
	if (UAbilitySystemComponent* ASC = GetInstigator()->FindComponentByClass<UAbilitySystemComponent>())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] Found ASC: %s"),
			*GetNameSafe(ASC));  // LogAdded
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] DamageEffectClass = %s, EffectLevel = %.2f"),
			*GetNameSafe(DamageEffectClass), EffectLevel);

		FGameplayEffectContextHandle Ctxt = ASC->MakeEffectContext();
		Ctxt.AddSourceObject(this);

		// DamageEffectClass, EffectLevel 은 BeginPlay()에서 이미 DataTable로 세팅되어 있음
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
			DamageEffectClass,
			EffectLevel,
			Ctxt
		);
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] SpecHandle created, valid=%s"),
			SpecHandle.IsValid() ? TEXT("true") : TEXT("false"));  // LogAdded

		if (SpecHandle.IsValid())
		{
			OwnerDamageEffectSpecHandle = SpecHandle;  // Blueprint로 꺼내 쓸 스펙 핸들
			UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] OwnerDamageEffectSpecHandle assigned"));  // LogAdded
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] No ASC found on instigator"));  // LogAdded
	}


	UE_LOG(LogTemp, Warning, TEXT("[ProjectileHit] CALL BP_PostProjectileHit"));
	BP_PostProjectileHit(OtherActor);
}


void AEnemyProjectileBase::OnCollisionOut(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	//Super::OnCollisionOut(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);


	BP_PostProjectileOut(OtherActor);
}