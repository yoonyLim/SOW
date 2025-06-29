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

	UFUNCTION(BlueprintCallable, Category = "Turret|Properties")
	void AddNewTargetPriority(ETurretTargetSelectionPriority NewPriority);

#pragma endregion

#pragma region DetectingFunction
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	void ClearTargetDetectionAsDead();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	FVector FindActualTargetLocation();
#pragma endregion



#pragma region GETTER
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	ETurretTargetSelectionPolicy GetTargetPolicy() const { return TurretTargetSelectionPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	ETurretTargetSelectionPriority GetTargetPriority() const { return TurretTargetSelectionPriority; }

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	float GetAttackCooldownTimeFromOwner() const;

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ATurretMeleeHitCollision* GetHitCollision() const { return CreatedHitCollision; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	TSubclassOf<ATurretProjectileBase> GetProjectileToSpawn() const { return ProjectileToSpawn; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ETurretName GetTurretNameByEnum() const { return TurretName; }

	FGameplayTag GetAbilityTagToActivation() const { return AbilityTagToActivation; }

	UFUNCTION(BlueprintPure, Category = "Turret|Combat")
	FWidgetDesciptableTurretAttribute GetWidgetDesciptableTurretAttribute() const { return WidgetDescriptableAttritutes; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	float GetProjectileLivingTime() const;
#pragma endregion

	

#pragma region SETTER
	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetHitCollision(ATurretMeleeHitCollision* HitCollsion);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetNewProjectile(TSubclassOf<ATurretProjectileBase> NewProjectile);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetWidgetDecriptableAttributes(const FWidgetDesciptableTurretAttribute& InAttribute);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetNewCollisionScale(float NewScale);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	bool SetFixedTarget(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetFixedLocation(const FVector InLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetNewProjectileLivingTime(float NewDuration);

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void SetNewTargetSelectCount(int32 NewCount);
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma region TurretCombatProperties

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
	TSubclassOf<ATurretProjectileBase> ProjectileToSpawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float ProjectileLivingTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float ProjectileMoveSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float ProjectileScaleRatio;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	int32 TargetSelectCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	FGameplayTag AbilityTagToActivation;
#pragma endregion

	
#pragma region TargetManagement
	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTarget();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTargetOnList(const TArray<AActor*> InTargetList);

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllAttackTarget();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<FVector> GetAllAttackLocation();

	UFUNCTION(BlueprintCallable)
	void AddNewFixedLocation(const FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	void ClearFixedLocationList();

	UFUNCTION(BlueprintCallable)
	bool TryAddNewFixedTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable)
	void ClearFixedTargetList();
#pragma endregion

	
	
private:

#pragma region InClassOnlyFields
	UPROPERTY()
	TArray<AActor*> DetectedTargetActors;

	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	UPROPERTY()
	AActor* FixedTarget;

	UPROPERTY()
	FVector FixedLocation;

	UPROPERTY()
	AActor* AttackTarget;

#pragma region Test
	UPROPERTY()
	TArray<AActor*> AttackTargetList;

	UPROPERTY()
	TArray<FVector> AttackTargetLocationList;

	UPROPERTY()
	TArray<AActor*> FixedTargetList;

	UPROPERTY()
	TArray<FVector> FixedLocationList;

	
#pragma endregion


	int32 CurrentPriorityNumber = 0;

	ASOWCharacterTurretBase* CachedOwnerCharacter;


	float M_CachedCooldownTime;
	float M_CachedDetectionRadius;

	bool HasDependencyOnProjectile;
	bool HasProjectileMovement;


	UPROPERTY(VisibleAnywhere, Category = "Turret|Combat")
	ATurretMeleeHitCollision* CreatedHitCollision;

	FWidgetDesciptableTurretAttribute WidgetDescriptableAttritutes;
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
