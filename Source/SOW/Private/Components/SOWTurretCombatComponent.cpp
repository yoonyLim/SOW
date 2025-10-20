// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Turrets/Actors/TurretMeleeHitCollision.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Interface/SOWCharacterUIInterface.h"
#include "Components/UI/SOWTurretUIComponent.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "Projectile/Turret/TurretProjectileBase.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "SOWBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SOWEnumTypes.h"
#include "Tile/TileBase.h"


USOWTurretCombatComponent::USOWTurretCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USOWTurretCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CachedOwnerCharacter = Cast<ASOWCharacterTurretBase>(GetOwner());
	CachedUIInterface = TWeakInterfacePtr<ISOWCharacterUIInterface>(CachedOwnerCharacter);

	

	if (!TurretSettablePriority.IsEmpty()) {
		PriorityChange();
	}
	//ActivateTurretFunction();

	USOWTurretUIComponent* UIComponent = CachedUIInterface ->GetTurretUIComponent();
	if (UIComponent) {
		UIComponent->PriorityChanged.AddDynamic(this, &USOWTurretCombatComponent::ChangePriorityCircular);
	}
	
}

float USOWTurretCombatComponent::GetAttackCooldownTimeFromOwner() const
{
	float CooldownBase = CachedOwnerCharacter->GetAttackCooldownTime();

	if (HasIndependantCooltime) {
		CooldownBase = IndependantCooltime;
	}

	if (HasDependencyOnProjectile) {
		CooldownBase += ProjectileLivingTime;
	}

	if (!bTargetFound) {
		CooldownBase = 0.25f;
	}

	return CooldownBase;
}

void USOWTurretCombatComponent::ClearTargetDetectionAsDead()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

bool USOWTurretCombatComponent::GetStealthCheck(AActor* Target) const
{
	if (AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Buff) {
		UE_LOG(LogTemp, Warning, TEXT("%s is not check stealth"), *Target->GetActorNameOrLabel());
		return false;
	}

	bool HasStealth = USOWBlueprintFunctionLibrary::DoesActorHasTag(Target, SOWGameplayTags::Enemy_Status_Buff_Stealth);
	bool CanDetectStealth = USOWBlueprintFunctionLibrary::DoesActorHasTag(CachedOwnerCharacter, SOWGameplayTags::Turret_Status_Buff_Detector);
	bool HasDetected = USOWBlueprintFunctionLibrary::DoesActorHasTag(Target, SOWGameplayTags::Enemy_Status_Debuff_Detected);

	return HasStealth && !(CanDetectStealth || HasDetected);
}


void USOWTurretCombatComponent::InitTurretProperties(const FTurretPropertyData& Data)
{
	// Need To Data Table

	// Enum Type
	TurretRarity = Data.TurretRarity;
	TurretSettablePriority = Data.TurretSettablePriority;
	TurretTargetSelectionPolicy = Data.TurretTargetSelectionPolicy;
	TurretTargetSelectionType = Data.TurretTargetSelectionType;
	TurretAttackType = Data.TurretAttackType;
	// Boolean Type
	ProjectileToSpawn = Data.ProjectileToSpawn;
	HasDependencyOnProjectile = Data.HasDependencyOnProjectile;
	HasIndependantCooltime = Data.HasIndependantCooltime;
	HasProjectileMovement = Data.HasProjectileMovement;

	// Numeric Type
	ProjectileLivingTime = Data.ProjectileLivingTime;
	ProjectileScaleRatio = Data.ProjectileScaleRatio;
	ProjectileMoveSpeed = Data.ProjectileMoveSpeed;
	IndependantCooltime = Data.IndependantCooltime;

	TargetSelectCount = Data.TargetSelectCount;

	if (!TurretSettablePriority.IsEmpty()) {
		PriorityChange();
	}

	/*if (Data.TurretAttackType == EAttackType::MELEE) {
		CachedOwnerCharacter->GetSOWAbilitySystemComponent()->AddLooseGameplayTag(SOWGameplayTags::Turret_Type_Melee);
	}
	else if(Data.TurretAttackType == EAttackType::RANGED){
		CachedOwnerCharacter->GetSOWAbilitySystemComponent()->AddLooseGameplayTag(SOWGameplayTags::Turret_Type_Ranged);
	}
	else if (Data.TurretAttackType == EAttackType::SUPPORT) {
		CachedOwnerCharacter->GetSOWAbilitySystemComponent()->AddLooseGameplayTag(SOWGameplayTags::Turret_Type_Support);
	}*/


}

void USOWTurretCombatComponent::AddNewTargetPriority(ETurretTargetSelectionPriority NewPriority)
{
	TurretSettablePriority.AddUnique(NewPriority);
	PriorityChange();
}

void USOWTurretCombatComponent::VisualizeTurretDetectionRange(bool bOn, TArray<ATileBase*>& OutTiles)
{

	if (DetectorTiles.Num() <= 0) {
		OutTiles = TArray<ATileBase*>();
		return;
	} 

	if (bOn) {
		for (ATileBase* Tile : DetectorTiles) {
			if (!Tile) continue;
			Tile->ShowRange();
		}
	}
	else {
		for (ATileBase* Tile : DetectorTiles) {
			if (!Tile) continue;
			Tile->HideRange();
		}
	}

	OutTiles = DetectorTiles;
}

void USOWTurretCombatComponent::MakeDetectableTileArea()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (!PC || !CachedOwnerCharacter) return;

	float Radius = 2 * CachedOwnerCharacter->GetDetectionRangeRadius() + 1;
	float TileSize = 116.f;

	FVector CPoint = USOWBlueprintFunctionLibrary::MakeCentralTileLocationFromAnyPoint(
		PC,
		CachedOwnerCharacter->GetActorLocation(),
		ETileSelectType::SQUARED,
		Radius,
		TileSize,
		true);

	DetectorTiles = USOWBlueprintFunctionLibrary::GetTilesAsSquaredFromCenterLocation(
		PC,
		CPoint,
		Radius,
		TileSize);
}

float USOWTurretCombatComponent::GetProjectileLivingTime() const
{
	float Duration;

	if (HasProjectileMovement) {
		Duration = 0.5f;
	}
	else {
		Duration = ProjectileLivingTime;
	}

	return Duration;
}

bool USOWTurretCombatComponent::GetActiveBool() const
{
	return bActive;
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

bool USOWTurretCombatComponent::SetFixedTarget(AActor* InActor)
{
	if (!IsValid(InActor) || !InActor->Implements<USOWCharacterTypeInterface>()) return false;
	ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(InActor);
	ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();

	

	bool bIsSuccess = USOWBlueprintFunctionLibrary::IsTarget(TurretTargetSelectionPolicy, TargetType);
	if (bIsSuccess) {
		if (ASOWCharacter* PreviousTarget = Cast<ASOWCharacter>(FixedTarget)) {
			PreviousTarget->OnTargetDead.RemoveAll(CachedOwnerCharacter);
		}

		/*if (FixedTarget) {
			UE_LOG(LogTemp, Warning, TEXT("Cur : %s, Target : %s"), *FixedTarget->GetActorNameOrLabel(), *InActor->GetActorNameOrLabel());
		}*/
		
		FixedTarget = InActor;
		AttackTarget = FixedTarget;
		//UE_LOG(LogTemp, Warning, TEXT("Target : %s"), *FixedTarget->GetActorNameOrLabel());
		CachedOwnerCharacter->BP_BindOnTargetDead(FixedTarget);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Target Unmatched with Targeting Policy"));
	}
	return bIsSuccess;
}

void USOWTurretCombatComponent::SetFixedLocation(const FVector InLocation)
{
	FixedLocation = InLocation;
}

void USOWTurretCombatComponent::SetNewProjectileLivingTime(float NewDuration)
{
	ProjectileLivingTime = NewDuration;
}

void USOWTurretCombatComponent::SetNewTargetSelectCount(int32 NewCount)
{
	TargetSelectCount = NewCount;
}

void USOWTurretCombatComponent::ActivateTurretFunction()
{
	MakeDetectableTileArea();
	//UE_LOG(LogTemp, Warning, TEXT("Tile Count : %s"), *FString::FromInt(DetectorTiles.Num()));
	CachedOwnerCharacter->SwitchCollision(true);
	RefreshTurretFunction();
}

void USOWTurretCombatComponent::PauseTurretFunction()
{
	DetectorTiles.Empty();
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	bActive = false;
	M_CachedCooldownTime = -1.f;

	CachedOwnerCharacter->SwitchCollision(false);
}

void USOWTurretCombatComponent::RefreshTurretFunction()
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
			NewCooldownTime
		);

		M_CachedCooldownTime = NewCooldownTime;
	}

	bActive = true;
}

void USOWTurretCombatComponent::SetManaConsumptionValue(float value)
{
	ManaConsumption = value;
}

bool USOWTurretCombatComponent::FindAttackTargetFromAllTargetAvailable()
{
	// Check all characters within detection range and designate them as attack targets if they are valid targets.

	TArray<AActor*> L_DetectableActors;
	DetectedTargetActors.Empty();


	if (!IsValid(CachedOwnerCharacter)) {
		//UE_LOG(LogTemp, Warning, TEXT("CachedOwnerCharacter is not initialized"));
		return (bTargetFound = false);
	}

	FVector TurretPosition = CachedOwnerCharacter->GetActorLocation();
	FVector Start = TurretPosition + FVector(0, 0, 500.f);
	FVector End = TurretPosition + FVector(0, 0, -500.f);

	//APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	FHitResult TileHit;

	if(AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Attack)
		L_DetectableActors = USOWBlueprintFunctionLibrary::GetActorsOnTiles(DetectorTiles);
	else if (AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Buff)
		L_DetectableActors = USOWBlueprintFunctionLibrary::GetTurretsOnTiles(DetectorTiles);
	else if(AbilityTagToActivation == SOWGameplayTags::Turret_Ability_Continue)
		return (bTargetFound = true);
	else {
		return (bTargetFound = false);
	}
	

	//UE_LOG(LogTemp, Warning, TEXT("%s : Detected Target Count : %s"), *CachedOwnerCharacter->GetActorNameOrLabel(), *FString::FromInt(L_DetectableActors.Num()));

	for (AActor* CurrentTarget : L_DetectableActors) {

		if (!IsActorValidTarget(CurrentTarget) ) {//|| GetStealthCheck(CurrentTarget)
			//UE_LOG(LogTemp, Warning, TEXT("%s is not target of %s"), *CurrentTarget->GetActorNameOrLabel(), *CachedOwnerCharacter->GetActorNameOrLabel());
			continue;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s is target of %s"), *CurrentTarget->GetActorNameOrLabel(), *CachedOwnerCharacter->GetActorNameOrLabel());

		if (CurrentTarget->Implements<USOWCharacterTypeInterface>()) {
			ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(CurrentTarget);
			ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();

			AddActorMatchesTargetingPolicy(CurrentTarget, TargetType);
		}
		
	}
	
	return bTargetFound = (!DetectedTargetActors.IsEmpty() || 
		TurretTargetSelectionPriority == ETurretTargetSelectionPriority::LocationFixed || 
		TurretTargetSelectionPriority == ETurretTargetSelectionPriority::TargetFixed)
		;
}


AActor* USOWTurretCombatComponent::GetSingleAttackTargetOnList(const TArray<AActor*> InTargetList)
{
	AActor* FinalTarget = CachedOwnerCharacter;
	double TargetValue, CriticValue;
	FVector TurretLocation = CachedOwnerCharacter->GetActorLocation();

	switch (TurretTargetSelectionPriority)
	{
	case ETurretTargetSelectionPriority::Uncertain:
		break;
	case ETurretTargetSelectionPriority::HighHealth:
		CriticValue = 0;

		for (AActor* ATarget : InTargetList) {

			if (!IsValid(ATarget)) continue;
			ASOWCharacterEnemyBase* TargetEnemy = Cast<ASOWCharacterEnemyBase>(ATarget);
			TargetValue = TargetEnemy->GetSOWAttibuteSet()->GetCurrentHealth();

			if (CriticValue < TargetValue) {
				FinalTarget = ATarget;
				CriticValue = TargetValue;
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("HighHealth / Target : %s"), *FinalTarget->GetActorNameOrLabel());
		break;
	case ETurretTargetSelectionPriority::LowHealth:
		CriticValue = 10000.0f;

		for (AActor* ATarget : InTargetList) {
			if (!IsValid(ATarget)) continue;
			ASOWCharacterEnemyBase* TargetEnemy = Cast<ASOWCharacterEnemyBase>(ATarget);
			TargetValue = TargetEnemy->GetSOWAttibuteSet()->GetCurrentHealth();

			if (CriticValue > TargetValue) {
				FinalTarget = ATarget;
				CriticValue = TargetValue;
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("LowHealth / Target : %s"), *FinalTarget->GetActorNameOrLabel());
		break;
	case ETurretTargetSelectionPriority::HighAttack:
		CriticValue = 0.f;

		for (AActor* ATarget : InTargetList) {
			if (!IsValid(ATarget)) continue;
			ASOWCharacterEnemyBase* TargetEnemy = Cast<ASOWCharacterEnemyBase>(ATarget);
			TargetValue = TargetEnemy->GetSOWAttibuteSet()->GetAttackPowerBase();

			if (CriticValue < TargetValue) {
				FinalTarget = ATarget;
				CriticValue = TargetValue;
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("HighAttack / Target : %s"), *FinalTarget->GetActorNameOrLabel());
		break;
	case ETurretTargetSelectionPriority::Nearest:
		CriticValue = 10000.f;

		for (AActor* ATarget : InTargetList) {
			if (!IsValid(ATarget)) continue;
			TargetValue = FVector::Dist(TurretLocation, ATarget->GetActorLocation());

			if (CriticValue > TargetValue) {
				FinalTarget = ATarget;
				CriticValue = TargetValue;
			}
		}
		break;
	case ETurretTargetSelectionPriority::Farthest:
		CriticValue = 0.f;

		for (AActor* ATarget : InTargetList) {
			TargetValue = FVector::Dist(TurretLocation, ATarget->GetActorLocation());

			if (CriticValue < TargetValue) {
				FinalTarget = ATarget;
				CriticValue = TargetValue;
			}
		}
		break;
	case ETurretTargetSelectionPriority::LocationFixed:
		FinalTarget = CachedOwnerCharacter;
		break;
	case ETurretTargetSelectionPriority::TargetFixed:
		FinalTarget = InTargetList[0];
		break;
	default:
		break;
	}

	return FinalTarget;
}

TArray<AActor*> USOWTurretCombatComponent::GetAllAttackTarget()
{
	// Get all attack targets that match the target selection count

	TArray<AActor*> BaseActorList;
	TArray<AActor*> FinalTargetList;

	if (TurretTargetSelectionPriority == ETurretTargetSelectionPriority::TargetFixed) {
		BaseActorList += FixedTargetList;
	}
	else {
		BaseActorList += DetectedTargetActors;
	}

	for (int i = 0; i < TargetSelectCount; i++) {
		if (BaseActorList.Num() <= 0) break;

		AActor* CurrentTarget = GetSingleAttackTargetOnList(BaseActorList);
		if (!IsValid(CurrentTarget)) break;

		if (!GetStealthCheck(CurrentTarget)) {
			FinalTargetList.AddUnique(CurrentTarget);
		}

		BaseActorList.Remove(CurrentTarget);
	}

	return FinalTargetList;
}
TArray<FVector> USOWTurretCombatComponent::GetAllAttackLocation()
{
	TArray<FVector> FinalLocationList;

	if (TurretTargetSelectionPriority == ETurretTargetSelectionPriority::LocationFixed) {
		FinalLocationList = FixedLocationList;
	}
	else {
		for (const AActor* CurActor : GetAllAttackTarget()) {
			if (!IsValid(CurActor)) continue;

			FinalLocationList.Add(CurActor->GetActorLocation());
		}
	}

	return FinalLocationList;
}
TArray<AActor*> USOWTurretCombatComponent::GetAllDetectedTarget()
{
	return DetectedTargetActors;
}
void USOWTurretCombatComponent::AddNewFixedLocation(ATileBase* HitTile, const FVector NewLocation, ETargetFixErrorType& Error)
{
	float Dist = FVector::Distance(NewLocation, CachedOwnerCharacter->GetActorLocation());

	FVector TraceStart = NewLocation;
	FVector TraceEnd = TraceStart + (FVector(0,0,-1) * 10000.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.bReturnPhysicalMaterial = false;

	if (!IsValid(HitTile))
	{
		Error = ETargetFixErrorType::INVALID_TARGET;
		return;
	}

	if (DetectorTiles.Find(HitTile) == -1) {
		Error = ETargetFixErrorType::OUT_OF_RANGE;
		return;
	}

	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(CachedOwnerCharacter, SOWGameplayTags::Shared_Status_Dead)) {
		Error = ETargetFixErrorType::TURRET_DEAD;
		return;
	}

	Error = ETargetFixErrorType::SUCCESS;
	FixedLocationList.Add(NewLocation);
}
void USOWTurretCombatComponent::ClearFixedLocationList()
{
	FixedLocationList.Empty();
}

bool USOWTurretCombatComponent::TryAddNewFixedTarget(AActor* NewTarget, ETargetFixErrorType& Error)
{
	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(CachedOwnerCharacter, SOWGameplayTags::Shared_Status_Dead)) {
		Error = ETargetFixErrorType::TURRET_DEAD;
		return false;
	}

	if (!IsValid(NewTarget) || !NewTarget->Implements<USOWCharacterTypeInterface>()) {
		Error = ETargetFixErrorType::INVALID_TARGET;
		return false;
	} 
	ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(NewTarget);
	ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();

	if (CachedOwnerCharacter->GetDetectionRangeRadius() < FVector::Distance(NewTarget->GetActorLocation(), CachedOwnerCharacter->GetActorLocation())) {
		Error = ETargetFixErrorType::OUT_OF_RANGE;
		return false;
	}

	if (!USOWBlueprintFunctionLibrary::IsTarget(TurretTargetSelectionPolicy, TargetType)) {
		Error = ETargetFixErrorType::INVALID_TARGET;
		return false;
	}

	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(NewTarget, SOWGameplayTags::Shared_Status_Dead)) {
		Error = ETargetFixErrorType::INVALID_TARGET;
		return false;
	}
	
	FixedTargetList.AddUnique(NewTarget);
	CachedOwnerCharacter->BP_BindOnTargetDead(NewTarget);
	Error = ETargetFixErrorType::SUCCESS;
	return true;
}

void USOWTurretCombatComponent::ClearFixedTargetList()
{
	for (AActor* PreviousBindTarget : FixedTargetList) {
		if (!IsValid(PreviousBindTarget)) continue;

		if (ASOWCharacter* PreviousTarget = Cast<ASOWCharacter>(PreviousBindTarget)) {
			PreviousTarget->OnTargetDead.RemoveAll(CachedOwnerCharacter);
		}
	}
	FixedTargetList.Empty();
}

void USOWTurretCombatComponent::AttackAbilityActivation()
{
	if (!IsValid(CachedOwnerCharacter)) return;

	if (!FindAttackTargetFromAllTargetAvailable()) {
		UE_LOG(LogTemp, Warning, TEXT("No Valid Actor Found"));
		return;
	} 
	
	CachedOwnerCharacter->TryActivateAbilityWithTagOnASC(AbilityTagToActivation);

	RefreshTurretFunction(); // -> Reactivate if cooldowntime has been updated.
}

bool USOWTurretCombatComponent::IsActorValidTarget(AActor* InActor)
{
	// if target actor is equal to self or selection policy is uncertain, otherwise the actor has dead state, 
	// then it can not be a target for the turret
	if (!IsValid(InActor)) return false;
	//UE_LOG(LogTemp, Warning, TEXT("Valid Actor"));

	

	if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::Uncertain) return false;
	//UE_LOG(LogTemp, Warning, TEXT("Matched Policy"));

	// Case 1. Policy was not selected

	if (InActor == CachedOwnerCharacter) return false;
	//UE_LOG(LogTemp, Warning, TEXT("Not Seld"));
	// Case 2. Detected target is itself

	if (USOWBlueprintFunctionLibrary::NativeDoesActorHasTag(InActor, SOWGameplayTags::Shared_Status_Dead)) return false;
	//UE_LOG(LogTemp, Warning, TEXT("Alive Target"));

	// Case 3. Target actor has dead
	//UE_LOG(LogTemp, Warning, TEXT("%s is valid Target"), *InActor->GetActorNameOrLabel());
	return true;
}

void USOWTurretCombatComponent::UpdateAttackTimer()
{
	// if attack cooldown has changed from GE or something, apply that time to attack timer
	// -> clear old timer and recreate timer with new cooldown
	ActivateTurretFunction();
}

void USOWTurretCombatComponent::AddActorMatchesTargetingPolicy(AActor* CurrentActor, ESOWCharacterType Type) {

	// check target type that matches to target selection policy.
	// if matched, insert the target to DetectedTargetActors

	if (!IsValid(CurrentActor)) return;

	if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnPlayer) {
		if (Type == ESOWCharacterType::Player) {
			DetectedTargetActors.AddUnique(CurrentActor);
		}
	}

	else if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnTurret) {
		if (Type == ESOWCharacterType::Turret) {

			//UE_LOG(LogTemp, Warning, TEXT("Turret Detected"));
			DetectedTargetActors.AddUnique(CurrentActor);
		}
	}

	else if (TurretTargetSelectionPolicy == ETurretTargetSelectionPolicy::OnEnemy) {
		if (Type == ESOWCharacterType::Enemy) {
			//USOWBlueprintFunctionLibrary::DoesActorHasTag(CurrentActor, SOWGameplayTags::Veiled)
			//USOWBlueprintFunctionLibrary::DoesActorHasTag(CurrentActor, SOWGameplayTags::Enemy_Status_Debuff_Detected);

			DetectedTargetActors.AddUnique(CurrentActor);
		}
	}

	else {
		UE_LOG(LogTemp, Warning, TEXT("Something wrong while setting target"));
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
	if (!CachedUIInterface.Get()) return;

	USOWTurretUIComponent* UIComponent = CachedUIInterface->GetTurretUIComponent();
	if (!UIComponent) return;

	UIComponent->OnPriorityChangedInTurret.Broadcast(TurretTargetSelectionPriority);
	
}
