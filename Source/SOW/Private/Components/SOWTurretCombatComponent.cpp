// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Turrets/Actors/TurretMeleeHitCollision.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Components/SOWCharacterUIComponent.h"
#include "Projectile/Turret/TurretProjectileBase.h"

#include "SOWBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SOWEnumTypes.h"


USOWTurretCombatComponent::USOWTurretCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USOWTurretCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CachedOwnerCharacter = Cast<ASOWCharacterTurretBase>(GetOwner());

	bool L_bHasCooldown = (AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Attack);

	if (!TurretSettablePriority.IsEmpty()) {
		PriorityChange();
	}

	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&USOWTurretCombatComponent::AttackAbilityActivation,
		L_bHasCooldown ? GetAttackCooldownTimeFromOwner() : 3.f,
		true,
		0.1f
	);

	USOWCharacterUIComponent* UIComponent = CachedOwnerCharacter->GetCharacterUIComponent();
	UIComponent->PriorityChanged.AddDynamic(this, &USOWTurretCombatComponent::ChangePriorityCircular);
}

float USOWTurretCombatComponent::GetAttackCooldownTimeFromOwner() const
{
	float CooldownBase = CachedOwnerCharacter->GetAttackCooldownTime();

	if (HasDependencyOnProjectile) {
		CooldownBase += ProjectileLivingTime;
	}

	return CooldownBase;
}

void USOWTurretCombatComponent::ClearTargetDetectionAsDead()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void USOWTurretCombatComponent::InitTurretProperties(const FTurretPropertyData& Data)
{
	// Need To Data Table

	// Settable in Widget
	TurretRarity = Data.TurretRarity;
	TurretSettablePriority = Data.TurretSettablePriority;
	TurretTargetSelectionPolicy = Data.TurretTargetSelectionPolicy;
	TurretTargetSelectionType = Data.TurretTargetSelectionType;

	// Settable from Ability
	ProjectileToSpawn = Data.ProjectileToSpawn;
	HasDependencyOnProjectile = Data.HasDependencyOnProjectile;
	HasProjectileMovement = Data.HasProjectileMovement;
	ProjectileLivingTime = Data.ProjectileLivingTime;

	ProjectileMoveSpeed = Data.ProjectileMoveSpeed;

	if (!TurretSettablePriority.IsEmpty()) {
		PriorityChange();
	}


}

float USOWTurretCombatComponent::GetProjectileLivingTime() const
{
	float Duration;

	if (HasProjectileMovement) {
		Duration = ProjectileMoveSpeed / CachedOwnerCharacter->GetDetectionRangeRadius();
	}
	else {
		Duration = ProjectileLivingTime;
	}

	return Duration;
}

void USOWTurretCombatComponent::SetHitCollision(ATurretMeleeHitCollision* HitCollsion)
{
	if (!HitCollsion) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Collision is Invalid"));
		return;
	}

	CreatedHitCollision = HitCollsion;
}

void USOWTurretCombatComponent::SetNewProjectile(TSubclassOf<ATurretProjectileBase> NewProjectile)
{
	checkf(NewProjectile, TEXT("New Projectile is invalid. Check your Property Setter Asset."));

	ProjectileToSpawn = NewProjectile;
}

void USOWTurretCombatComponent::SetWidgetDecriptableAttributes(const FWidgetDesciptableTurretAttribute& InAttribute)
{
	// this struct must be constant values. once the attributes were initialized, base and ratio value will be fixed based on attribute set value.
	// this struct was assigned for describe some attributes on widget and process for variable properties due to gameplay effects
	WidgetDescriptableAttritutes = InAttribute;
}

void USOWTurretCombatComponent::SetNewCollisionScale(float NewScale)
{
	ProjectileScaleRatio = NewScale;
}

bool USOWTurretCombatComponent::FindAttackTargetFromAllTargetAvailable()
{
	// Check all characters within detection range and designate them as attack targets if they are valid targets.

	TArray<AActor*> L_DetectableActors;
	DetectedTargetActors.Empty();

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

		AddActorMatchesTargetingPolicy(CurrentTarget, TargetType);
	}
	UpdateAttackTimer();
	return !DetectedTargetActors.IsEmpty();
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

AActor* USOWTurretCombatComponent::GetSingleAttackTarget()
{
	if (AttackTarget && DetectedTargetActors.Contains(AttackTarget)) {
		return AttackTarget;
	} 

	FVector TurretLocation = CachedOwnerCharacter->GetActorLocation();

	double DistanceMin, DistanceMax, Distance;

	switch (TurretTargetSelectionPriority)
	{
	case ETurretTargetSelectionPriority::Uncertain:
		AttackTarget = nullptr;
		break;
	case ETurretTargetSelectionPriority::HighHealth:
		break;
	case ETurretTargetSelectionPriority::LowHealth:
		break;
	case ETurretTargetSelectionPriority::HighAttack:
		break;
	case ETurretTargetSelectionPriority::Nearest:
		DistanceMax = CachedOwnerCharacter->GetDetectionRangeRadius() * 2.f;

		for (AActor* ATarget : DetectedTargetActors) {
			Distance = FVector::Dist(TurretLocation, ATarget->GetActorLocation());

			if (DistanceMax > Distance) {
				AttackTarget = ATarget;
				DistanceMax = Distance;
			}
		}
		break;
	case ETurretTargetSelectionPriority::Farthest:
		DistanceMin = 0.f;

		for (AActor* ATarget : DetectedTargetActors) {
			Distance = FVector::Dist(TurretLocation, ATarget->GetActorLocation());

			if (DistanceMin < Distance) {
				AttackTarget = ATarget;
				DistanceMin = Distance;
			}
		}
		break;
	case ETurretTargetSelectionPriority::LocationFixed:
		break;
	case ETurretTargetSelectionPriority::TargetFixed:
		break;
	default:
		break;
	}
	return AttackTarget;
}

TArray<AActor*> USOWTurretCombatComponent::GetAllAttackTarget() const
{
	return DetectedTargetActors;
}
void USOWTurretCombatComponent::AttackAbilityActivation()
{
	if (!CachedOwnerCharacter) return;

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

void USOWTurretCombatComponent::ChangePriorityCircular(bool ToLeft)
{
	int32 PriorityCount = TurretSettablePriority.Num();
	if (PriorityCount <= 0) return;

	if (ToLeft) {
		if (CurrentPriorityNumber < 1) {
			CurrentPriorityNumber = PriorityCount;
		}
		CurrentPriorityNumber--;
	}
	else {
		if (CurrentPriorityNumber == PriorityCount - 1) {
			CurrentPriorityNumber -= PriorityCount;
		}
		CurrentPriorityNumber++;
	}

	PriorityChange();
}

void USOWTurretCombatComponent::PriorityChange()
{
	
	TurretTargetSelectionPriority = TurretSettablePriority[CurrentPriorityNumber];
	if (!CachedOwnerCharacter) return;

	USOWCharacterUIComponent* UIComponent = CachedOwnerCharacter->GetCharacterUIComponent();
	if (!UIComponent) return;
		
	UIComponent->OnPriorityChangedInTurret.Broadcast(TurretTargetSelectionPriority);
	
}
/