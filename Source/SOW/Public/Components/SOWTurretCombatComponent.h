// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"
#include "SOWGameplayTags.h"
#include "SOWTurretCombatComponent.generated.h"


class ASOWCharacterTurretBase;
class ATurretMeleeHitCollision;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWTurretCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWTurretCombatComponent();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties|Information")
	ETurretName TurretName;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Information")
	ETurretRarity TurretRarity = ETurretRarity::Common;

	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Priority")
	ETurretTargetSelectionPriority TurretTargetSelectionPriority = ETurretTargetSelectionPriority::Nearest;
	// Attack Target Selection Policy - Nearest Target Base

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Priority")
	TArray<ETurretTargetSelectionPriority> TurretSettablePriority;
	// This is not modifiable by default, but can be modified for debugging purposes.


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties")
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy = ETurretTargetSelectionPolicy::Uncertain;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties")
	ETurretTargetSelectionType TurretTargetSelectionType = ETurretTargetSelectionType::Single;


	

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool SelectAttackTarget();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool SelectNextAttackTarget();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	float GetAttackCooldownTimeFromOwner() const;

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	void ClearTargetDetectionAsDead();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	ETurretTargetSelectionPolicy GetTargetPolicy() const { return TurretTargetSelectionPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Turret|InitProperties")
	void InitTurretProperties(const FTurretPropertyData& Data);


#pragma region AboutHitCollision
	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetHitCollision(ATurretMeleeHitCollision* HitCollsion);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ATurretMeleeHitCollision* GetHitCollision() const;
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTarget();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetNextSingleAttackTarget() const;

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllAttackTarget() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	FGameplayTag AbilityTagToActivation;


private:
	UPROPERTY()
	TArray<AActor*> DetectedTargetActors;

	UPROPERTY()
	AActor* AttackTarget;

	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	ASOWCharacterTurretBase* CachedOwnerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Turret|Combat")
	ATurretMeleeHitCollision* CreatedHitCollision;

	void AttackAbilityActivation();
	bool IsActorValidTarget(AActor* InActor);

	void UpdateAttackTimer();
	void AddActorMatchesTargetingPolicy(AActor* CurrentActor, ESOWCharacterType Type);

	float M_CachedCooldownTime;
	float M_CachedDetectionRadius;
		
};
