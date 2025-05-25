// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SOWTurretCombatComponent.h"

#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWEnumTypes.h"


ASOWCharacterTurretBase::ASOWCharacterTurretBase()
{
	check(AttributeSet);

	//DetectionRange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectionRange"));
	//DetectionRange->SetupAttachment(RootComponent);
	//// 해당 범위는 콜리션 뿐만 아니라 공격 직전 수행해야 할 RayTracing 과정에도 사용
	//DetectionRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//// 활성화될 경우 콜리션을 QueryOnly로 설정하여 오버랩된 대상을 체크하는 용도로 활용.

	//DetectionRange->SetCapsuleHalfHeight(300.f);
	//// 콜리션 높이 설정. 하드코딩된 상태이므로 추후 변수화 작업 필요.

	//DetectionRange->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTargetRangeBeginOverlap);

	TurretCombatComponent = CreateDefaultSubobject<USOWTurretCombatComponent>(TEXT("TurretCombatComponent"));

	CharacterType = ESOWCharacterType::Turret;
	//AttackTarget = nullptr;
}

void ASOWCharacterTurretBase::BeginPlay()
{
	Super::BeginPlay();

	//if (AttributeSet) {
	//	M_CachedDetectionRadius = AttributeSet->GetDetectionRange();
	//	//SetDetectionRangeWithCurrentStatus();
	//}
}

void ASOWCharacterTurretBase::TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation)
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->TryActivateAbilityWithTag(InAbilityTagToActivation);
}

float ASOWCharacterTurretBase::GetDetectionRangeRadius() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));

	return AttributeSet->GetDetectionRange();
}

float ASOWCharacterTurretBase::GetAttackCooldownTime() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));

	return 1.0f / AttributeSet->GetAttackSpeed();
}

//void ASOWCharacterTurretBase::OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent,
//	AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,
//	int32 OtherBodyIndex,
//	bool bFromSweep,
//	const FHitResult& SweepResult)
//{
//
//	//check(TurretCombatComponent);
//	//TurretCombatComponent->FindAttackTargetFromAllTargetAvailable();
//	//FindAttackTargetFromAllTargetAvailable();
//}
//
//void ASOWCharacterTurretBase::OnTargetRangeEndOverlap(AActor* InTargetActor)
//{
//}
//
//void ASOWCharacterTurretBase::SetDetectionRangeWithCurrentStatus()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Range : %f"), M_CachedDetectionRadius);
//	DetectionRange->SetCapsuleRadius(M_CachedDetectionRadius);
//}

	
