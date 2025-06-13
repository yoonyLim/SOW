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
class ATurretProjectileBase;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWTurretCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWTurretCombatComponent();

#pragma region TurretProperties
	
	UFUNCTION(BlueprintCallable, Category = "Turret|InitProperties")
	void InitTurretProperties(const FTurretPropertyData& Data);

#pragma endregion

#pragma region DetectingFunction
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool SelectAttackTarget();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool SelectNextAttackTarget();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	void ClearTargetDetectionAsDead();
#pragma endregion



#pragma region GETTER
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	ETurretTargetSelectionPolicy GetTargetPolicy() const { return TurretTargetSelectionPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	float GetAttackCooldownTimeFromOwner() const;

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ATurretMeleeHitCollision* GetHitCollision() const { return CreatedHitCollision; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ATurretProjectileBase* GetProjectileToSpawn() const { return ProjectileToSpawn; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ETurretName GetTurretNameByEnum() const { return TurretName; }

	FGameplayTag GetAbilityTagToActivation() const { return AbilityTagToActivation; }
#pragma endregion

	

#pragma region SETTER
	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetHitCollision(ATurretMeleeHitCollision* HitCollsion);

	
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties|Information")
	ETurretName TurretName;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Information")
	ETurretRarity TurretRarity = ETurretRarity::Common;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Priority")
	ETurretTargetSelectionPriority TurretTargetSelectionPriority = ETurretTargetSelectionPriority::Uncertain;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Priority")
	TArray<ETurretTargetSelectionPriority> TurretSettablePriority;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy = ETurretTargetSelectionPolicy::Uncertain;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	ETurretTargetSelectionType TurretTargetSelectionType = ETurretTargetSelectionType::Single;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	ATurretProjectileBase* ProjectileToSpawn;

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTarget();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllAttackTarget() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	FGameplayTag AbilityTagToActivation;

	UPROPERTY()
	AActor* AttackTarget;

	int32 CurrentPriorityNumber = 0;
private:

#pragma region InClassOnlyFields
	UPROPERTY()
	TArray<AActor*> DetectedTargetActors;

	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	ASOWCharacterTurretBase* CachedOwnerCharacter;


	float M_CachedCooldownTime;
	float M_CachedDetectionRadius;

	UPROPERTY(VisibleAnywhere, Category = "Turret|Combat")
	ATurretMeleeHitCollision* CreatedHitCollision;
#pragma endregion


#pragma region InClassOnlyMethods
	void AttackAbilityActivation();
	bool IsActorValidTarget(AActor* InActor);

	void UpdateAttackTimer();
	void AddActorMatchesTargetingPolicy(AActor* CurrentActor, ESOWCharacterType Type);



	UFUNCTION()
	void ChangePriorityCircular(bool ToLeft);
	void PriorityChange();
#pragma endregion



	
};
