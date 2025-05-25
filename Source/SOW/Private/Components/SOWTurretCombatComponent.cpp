// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
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

bool USOWTurretCombatComponent::FindAttackTargetFromAllTargetAvailable()
{

	TArray<AActor*> L_DetectableActors;
	DetectedTargetActors.Empty();
	AttackTarget = nullptr;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UE_LOG(LogTemp, Warning, TEXT("Try to detect near actors"));

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
	return (DetectedTargetActors.Num() >= 1) ? DetectedTargetActors[0] : nullptr;
}

AActor* USOWTurretCombatComponent::GetNextSingleAttackTarget() const
{
	return (DetectedTargetActors.Num() >= 2) ? DetectedTargetActors[1] : nullptr;
}

TArray<AActor*> USOWTurretCombatComponent::GetAllAttackTarget() const
{
	return DetectedTargetActors;
}


void USOWTurretCombatComponent::AttackAbilityActivation()
{
	if (!IsActivated) return;

	if(!FindAttackTargetFromAllTargetAvailable()) return;

	if (!CachedOwnerCharacter) return;

	CachedOwnerCharacter->TryActivateAbilityWithTagOnASC(AbilityTagToActivation);
}

bool USOWTurretCombatComponent::IsActorValidTarget(AActor* InActor)
{
	if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::Uncertain) return false;
	// Case 1. Policy was not selected

	if (InActor == GetOwner()) return false;
	// Case 2. Detected target is itself

	if (USOWBlueprintFunctionLibrary::NativeDoesActorHasTag(InActor, SOWGameplayTags::Shared_Status_Dead)) return false;
	// Case 3. Target actor has dead

	return true;
}

void USOWTurretCombatComponent::UpdateAttackTimer()
{
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
