// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Turrets/Actors/TurretMeleeHitCollision.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"

#include "SOWBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SOWEnumTypes.h"

// Sets default values for this component's properties
USOWTurretCombatComponent::USOWTurretCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USOWTurretCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CachedOwnerCharacter = Cast<ASOWCharacterTurretBase>(GetOwner());

	bool L_bHasCooldown = (AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Attack);

	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&USOWTurretCombatComponent::AttackAbilityActivation,
		L_bHasCooldown ? GetAttackCooldownTimeFromOwner() : 3.f,
		true,
		0.1f
	);
}

float USOWTurretCombatComponent::GetAttackCooldownTimeFromOwner() const
{
	return CachedOwnerCharacter->GetAttackCooldownTime();
}

void USOWTurretCombatComponent::ClearTargetDetectionAsDead()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void USOWTurretCombatComponent::SetHitCollision(ATurretMeleeHitCollision* HitCollsion)
{
	if (!HitCollsion) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Collision is Invalid"));
		return;
	}

	CreatedHitCollision = HitCollsion;
}

ATurretMeleeHitCollision* USOWTurretCombatComponent::GetHitCollision() const
{
	return CreatedHitCollision;
}



bool USOWTurretCombatComponent::FindAttackTargetFromAllTargetAvailable()
{
	// Check all characters within detection range and designate them as attack targets if they are valid targets.

	TArray<AActor*> L_DetectableActors;
	DetectedTargetActors.Empty();
	AttackTarget = nullptr;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	if (!CachedOwnerCharacter) return false;


	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		CachedOwnerCharacter->GetActorLocation(),
		CachedOwnerCharacter->GetDetectionRangeRadius(),
		ObjectTypes,
		nullptr,
		TArray<AActor*>(),
		L_DetectableActors
	);

	for (AActor* CurrentTarget : L_DetectableActors) {

		if (!IsActorValidTarget(CurrentTarget)) continue;
		// if target actor is equal to self or selection policy is uncertain, otherwise the actor has dead state, 
		// then it can not be a target for the turret
		ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(CurrentTarget);
		ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();


		//AddActorMatchesTargetingPolicy(CurrentTarget, TargetType);
		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnPlayer) {
			if (Cast<ASOWCharacterPlayer>(CurrentTarget)) {
				DetectedTargetActors.AddUnique(CurrentTarget);
			}
		}

		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnTurret) {
			if (TargetType == ESOWCharacterType::Turret) {
				DetectedTargetActors.AddUnique(CurrentTarget);
			}
		}

		if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnEnemy) {
			if (Cast<ASOWCharacterEnemyBase>(CurrentTarget)) {
				DetectedTargetActors.AddUnique(CurrentTarget);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Attack Count : %s"), *FString::FromInt(DetectedTargetActors.Num()));
	UpdateAttackTimer();
	return (DetectedTargetActors.Num() >= 1) ? true : false;
}

bool USOWTurretCombatComponent::SelectAttackTarget()
{
	if (!CachedOwnerCharacter) return false;
	FVector TurretLocation = CachedOwnerCharacter->GetActorLocation();
	return AttackTarget ? true : false;
}

bool USOWTurretCombatComponent::SelectNextAttackTarget()
{
	return false;
}

AActor* USOWTurretCombatComponent::GetSingleAttackTarget() const
{
	AActor* TargetActor = nullptr;
	if (DetectedTargetActors.IsEmpty()) {
		return nullptr;
	} 

	FVector TurretLocation = CachedOwnerCharacter->GetActorLocation();

	if (TurretTargetSelectionPriority == ETurretTargetSelectionPriority::Nearest) {
		double DistanceMax = CachedOwnerCharacter->GetDetectionRangeRadius() * 2.f;

		for (AActor* ATarget : DetectedTargetActors) {
			double Distance = FVector::Dist(TurretLocation, ATarget->GetActorLocation());

			if (DistanceMax > Distance) {
				TargetActor = ATarget;
				DistanceMax = Distance;
			}
		}
	}

	return TargetActor;
}

AActor* USOWTurretCombatComponent::GetNextSingleAttackTarget() const
{
	// unusing function
	return (DetectedTargetActors.Num() >= 2) ? DetectedTargetActors[1] : nullptr;
}

TArray<AActor*> USOWTurretCombatComponent::GetAllAttackTarget() const
{
	return DetectedTargetActors;
}


void USOWTurretCombatComponent::AttackAbilityActivation()
{
	if (!CachedOwnerCharacter) return;

	//if (!CachedOwnerCharacter->bIsActivated) return;

	if(!FindAttackTargetFromAllTargetAvailable()) return;

	CachedOwnerCharacter->TryActivateAbilityWithTagOnASC(AbilityTagToActivation);
}

bool USOWTurretCombatComponent::IsActorValidTarget(AActor* InActor)
{
	if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::Uncertain) {
		return false;
	} 
	// Case 1. Policy was not selected

	if (InActor == GetOwner()) {
		return false;
	} 
	// Case 2. Detected target is itself

	if (USOWBlueprintFunctionLibrary::NativeDoesActorHasTag(InActor, SOWGameplayTags::Shared_Status_Dead)) {
		return false;
	}
	// Case 3. Target actor has dead

	return true;
}

void USOWTurretCombatComponent::UpdateAttackTimer()
{
	// if attack cooldown has changed from GE or something, apply that time to attack timer
	// -> clear old timer and recreate timer with new cooldown

	bool bIsFixedCooldown = (AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Attack);

	const float NewCooldownTime = bIsFixedCooldown ? GetAttackCooldownTimeFromOwner() : 3.f;

	if (M_CachedCooldownTime != NewCooldownTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&USOWTurretCombatComponent::AttackAbilityActivation,
			NewCooldownTime,
			true,
			0.1f
		);

		M_CachedCooldownTime = NewCooldownTime;
	}
}

void USOWTurretCombatComponent::AddActorMatchesTargetingPolicy(AActor* CurrentActor, ESOWCharacterType Type) {

	// check target type that matches to target selection policy.
	// if matched, insert the target to DetectedTargetActors

	if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnPlayer) {
		if (Type == ESOWCharacterType::Player) {
			DetectedTargetActors.AddUnique(CurrentActor);
		}
	}

	else if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnTurret) {
		if (Type == ESOWCharacterType::Turret) {
			DetectedTargetActors.AddUnique(CurrentActor);
		}
	}

	else if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnEnemy) {
		if (Type == ESOWCharacterType::Enemy) {
			DetectedTargetActors.AddUnique(CurrentActor);
		}
	}
}
