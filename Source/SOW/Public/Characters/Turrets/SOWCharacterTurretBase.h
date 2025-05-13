// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWGameplayTags.h"
#include "SOWCharacterTurretBase.generated.h"

class UCapsuleComponent;

UENUM(BlueprintType)
enum class ETurretTargetSelectionPolicy : uint8 {
	HighHealth,
	LowHealth,
	HighAttack,
	Nearest,
	Farthest,
	Custom
};

UENUM(BlueprintType)
enum class ETurretRarity : uint8 {
	Common,
	Uncommon,
	Rare,
	Epic
};
/**
 * 
 */
UCLASS()
class SOW_API ASOWCharacterTurretBase : public ASOWCharacter
{
	GENERATED_BODY()
	
public:
	ASOWCharacterTurretBase();

	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystem")
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy = ETurretTargetSelectionPolicy::Nearest;
	// Attack Target Selection Policy - Nearest Target Base

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Status")
	ETurretRarity TurretRarity = ETurretRarity::Common;

	

protected:
	UFUNCTION(BlueprintCallable, Category = "Turret|Ability")
	float GetAttackCooldownTime() const;

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTarget() const;

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllAttackTarget() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UCapsuleComponent* DetectionRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	FGameplayTag AbilityTagToActivation;

private:
	/* Callback Function for Collision Overlap */
	UFUNCTION()
	void OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetRangeEndOverlap(AActor* InTargetActor);
	// Need to Change Parameters to Bind it.

	/* Target Detection / Attack Properties Begin */
	UPROPERTY()
	TArray<AActor*> DetectedTargetActors;

	UPROPERTY()
	AActor* AttackTarget;

	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	void AttackAbilityActivation();
	void SetDetectionRangeWithCurrentStatus();

	float M_CachedDetectionRadius = 0.f;
	/* Target Detection / Attack Properties End */
};
