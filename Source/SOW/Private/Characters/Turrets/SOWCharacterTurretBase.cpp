// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"


ASOWCharacterTurretBase::ASOWCharacterTurretBase()
{
	check(AttributeSet);
	//

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

	M_CachedDetectionRadius = AttributeSet->GetDetectionRange();
	SetDetectionRangeWithCurrentStatus();
}

float ASOWCharacterTurretBase::GetAttackCooldownTime() const
{
	check(AttributeSet);

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
		DetectedTargetActors
	);

	UE_LOG(LogTemp, Warning, TEXT("%s Detected %s Actors."), *GetActorNameOrLabel(), *FString::FromInt(DetectedTargetActors.Num()));

	for (AActor* CurrentTarget : DetectedTargetActors) {

		// TO-DO 
		// 타겟 지정 정책에 따라 액터의 스텟 정보를 호출해
		// 정책과 맞는 스텟을 가진 액터를 타겟 액터로 지정

		// Below Codes are for debugging : can a turret detects Player Character and activates its ability
		if (Cast<ASOWCharacterPlayer>(CurrentTarget)) {
			AttackTarget = CurrentTarget;
		}
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