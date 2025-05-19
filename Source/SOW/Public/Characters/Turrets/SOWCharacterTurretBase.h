// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWGameplayTags.h"
#include "SOWEnumTypes.h"
#include "SOWCharacterTurretBase.generated.h"

class UCapsuleComponent;

UENUM(BlueprintType)
enum class ETurretTargetSelectionPriority : uint8 {
	HighHealth,
	LowHealth,
	HighAttack,
	Nearest,
	Farthest,
	Custom
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
	virtual void Tick(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Property")
	ETurretTargetSelectionPriority TurretTargetSelectionPriority = ETurretTargetSelectionPriority::Nearest;
	// Attack Target Selection Policy - Nearest Target Base

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Property")
	ETurretRarity TurretRarity = ETurretRarity::Common;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Property")
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy = ETurretTargetSelectionPolicy::Uncertain;

protected:
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	float GetAttackCooldownTime() const;

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTarget() const;

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllAttackTarget() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCapsuleComponent* DetectionRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Property")
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
