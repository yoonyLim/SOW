// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWEnumTypes.h"
#include "SOWGameplayTags.h"
#include "SOWTurretCombatComponent.generated.h"


class ASOWCharacterTurretBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWTurretCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWTurretCombatComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Properties")
	ETurretTargetSelectionPriority TurretTargetSelectionPriority = ETurretTargetSelectionPriority::Nearest;
	// Attack Target Selection Policy - Nearest Target Base

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Properties")
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy = ETurretTargetSelectionPolicy::Uncertain;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Properties")
	ETurretTargetSelectionType TurretTargetSelectionType = ETurretTargetSelectionType::Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Properties")
	bool IsActivated = true;

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool SelectAttackTarget();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool SelectNextAttackTarget();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	float GetAttackCooldownTimeFromOwner() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTarget() const;

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

	void AttackAbilityActivation();
	bool IsActorValidTarget(AActor* InActor);

	void UpdateAttackTimer();
	void AddActorMatchesTargetingPolicy(AActor* CurrentActor, ESOWCharacterType Type);

	float M_CachedCooldownTime;
	float M_CachedDetectionRadius;
		
};
