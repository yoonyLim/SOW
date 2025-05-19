// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"


ASOWCharacterTurretBase::ASOWCharacterTurretBase()
{
	check(AttributeSet);
	//
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetMesh()->bReceivesDecals = false;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	GetMesh()->PrimaryComponentTick.bCanEverTick = true;
	GetMesh()->PrimaryComponentTick.bStartWithTickEnabled = true;

	DetectionRange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectionRange"));
	DetectionRange->SetupAttachment(RootComponent);
	// 해당 범위는 콜리션 뿐만 아니라 공격 직전 수행해야 할 RayTracing 과정에도 사용
	DetectionRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 활성화될 경우 콜리션을 QueryOnly로 설정하여 오버랩된 대상을 체크하는 용도로 활용.

	DetectionRange->SetCapsuleHalfHeight(300.f);
	// 콜리션 높이 설정. 하드코딩된 상태이므로 추후 변수화 작업 필요.

	DetectionRange->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTargetRangeBeginOverlap);

	AttackTarget = nullptr;
}

void ASOWCharacterTurretBase::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeSet) {
		M_CachedDetectionRadius = AttributeSet->GetDetectionRange();
		SetDetectionRangeWithCurrentStatus();
	}
}


void ASOWCharacterTurretBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Ticking"));
}
float ASOWCharacterTurretBase::GetAttackCooldownTime() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));

	return 1.0f / AttributeSet->GetAttackSpeed();
}

void ASOWCharacterTurretBase::OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 타겟이 최초로 감지 범위에 진입한 경우에만 타겟을 탐색해야함.
	// 모든 탐색 행위는 능력 수행 전에 이루어짐.

	UE_LOG(LogTemp, Warning, TEXT("Target : %s has entered in Detection Range."), *OtherActor->GetActorNameOrLabel());

//	if (AttackTarget) return;

	FindAttackTargetFromAllTargetAvailable();
}

void ASOWCharacterTurretBase::OnTargetRangeEndOverlap(AActor* InTargetActor)
{
}

bool ASOWCharacterTurretBase::FindAttackTargetFromAllTargetAvailable()
{
	TArray<AActor*> L_DetectableActors;
	DetectedTargetActors.Empty();
	AttackTarget = nullptr;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		M_CachedDetectionRadius,
		ObjectTypes,
		nullptr,
		TArray<AActor*>(),
		L_DetectableActors
	);

	UE_LOG(LogTemp, Warning, TEXT("Detected Actors Count : %s"), *FString::FromInt(L_DetectableActors.Num()));

	for (AActor* CurrentTarget : L_DetectableActors) {
		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::Uncertain || CurrentTarget == this) continue;

		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnPlayer) {
			if (Cast<ASOWCharacterPlayer>(CurrentTarget)) {
				DetectedTargetActors.AddUnique(CurrentTarget);
			}
		}

		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnTurret) {
			if (Cast<ASOWCharacterTurretBase>(CurrentTarget)) {
				DetectedTargetActors.AddUnique(CurrentTarget);
			}
		}

		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnEnemy) {
			if (Cast<ASOWCharacterEnemyBase>(CurrentTarget)) {
				DetectedTargetActors.AddUnique(CurrentTarget);
			}
		}
		// TO-DO 
		// 타겟 지정 정책에 따라 액터의 스텟 정보를 호출해
		// 정책과 맞는 스텟을 가진 액터를 타겟 액터로 지정

		// Below Codes are for debugging : can a turret detects Player Character and activates its ability
		UE_LOG(LogTemp, Warning, TEXT("Selectable Actors Count : %s"), *FString::FromInt(DetectedTargetActors.Num()));
		AttackTarget = (DetectedTargetActors.Num() >= 1) ? DetectedTargetActors[0] : nullptr;
		
		
	}


	if (!AttackTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&ASOWCharacterTurretBase::AttackAbilityActivation,
			GetAttackCooldownTime(),
			true
		);
	}
	

	return AttackTarget ? true : false;
}

AActor* ASOWCharacterTurretBase::GetSingleAttackTarget() const
{
	return AttackTarget;
}

TArray<AActor*> ASOWCharacterTurretBase::GetAllAttackTarget() const
{
	return DetectedTargetActors;
}

void ASOWCharacterTurretBase::SetDetectionRangeWithCurrentStatus()
{
	UE_LOG(LogTemp, Warning, TEXT("Range : %f"), M_CachedDetectionRadius);
	DetectionRange->SetCapsuleRadius(M_CachedDetectionRadius);

}

void ASOWCharacterTurretBase::AttackAbilityActivation() {
	FindAttackTargetFromAllTargetAvailable();	

	if (!AttackTarget) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Current Target : %s"), *AttackTarget->GetActorNameOrLabel());

	AbilitySystemComponent->TryActivateAbilityWithTag(AbilityTagToActivation);
}