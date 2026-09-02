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
class ISOWCharacterUIInterface;
class ATileBase;



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

	void VisualizeTurretDetectionRange(bool bOn, TArray<ATileBase*>& OutTiles);

	void MakeDetectableTileArea();
#pragma endregion

#pragma region DetectingFunction
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	bool FindAttackTargetFromAllTargetAvailable();

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	void ClearTargetDetectionAsDead();

	bool GetStealthCheck(AActor* Target) const;

#pragma endregion



#pragma region GETTER
	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	ETurretTargetSelectionPolicy GetTargetPolicy() const { return TurretTargetSelectionPolicy; }

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	ETurretTargetSelectionPriority GetTargetPriority() const { return TurretTargetSelectionPriority; }

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	ETurretRarity GetTurretRarity() const { return TurretRarity; };

	UFUNCTION(BlueprintCallable, Category = "Turret|TargetDetection")
	float GetAttackCooldownTimeFromOwner() const;

	UFUNCTION(BlueprintPure, Category = "Turret|Combat")
	EAttackType GetTurretAttackType() const { return TurretAttackType; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ATurretMeleeHitCollision* GetHitCollision() const { return CreatedHitCollision; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	TSubclassOf<ATurretProjectileBase> GetProjectileToSpawn() const { return ProjectileToSpawn; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	ETurretName GetTurretNameByEnum() const { return TurretName; }

	UFUNCTION(BlueprintPure, Category = "Turret|Combat")
	FWidgetDesciptableTurretAttribute GetWidgetDesciptableTurretAttribute() const { return WidgetDescriptableAttritutes; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	float GetProjectileLivingTime() const;

	UFUNCTION(BlueprintCallable, Category = "Turret|Property")
	FString GetTurretDescriptor(int idx = 0) const{ 
		if (TurretDescriptor.Num() <= 0) return "";

			return TurretDescriptor[idx];
	} 

	



	bool GetActiveBool() const;
	FGameplayTag GetAbilityTagToActivation() const { return AbilityTagToActivation; }
	EGlacioStatType GetAffectStatType() const { return AffectStatType; }
	float GetAffectStatValue() const {return AffectStatValue;}
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

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void ActivateTurretFunction();

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void PauseTurretFunction();

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	void RefreshTurretFunction();

	void SetManaConsumptionValue(float value);
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
	EAttackType TurretAttackType = EAttackType::UNDEFINED;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	TSubclassOf<ATurretProjectileBase> ProjectileToSpawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float ProjectileLivingTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float ProjectileMoveSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float ProjectileScaleRatio;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	float IndependantCooltime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Turret|Properties|Combat")
	int32 TargetSelectCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	FGameplayTag AbilityTagToActivation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	EGlacioStatType AffectStatType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	float AffectStatValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Properties")
	TArray<FString> TurretDescriptor;
#pragma endregion

	
#pragma region TargetManagement
	
	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	AActor* GetSingleAttackTargetOnList(const TArray<AActor*> InTargetList);

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllAttackTarget();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<FVector> GetAllAttackLocation();

	UFUNCTION(BlueprintPure, Category = "Turret|TargetDetection")
	TArray<AActor*> GetAllDetectedTarget();

	UFUNCTION(BlueprintCallable)
	void AddNewFixedLocation(ATileBase* HitTile,const FVector NewLocation, ETargetFixErrorType& Error);

	UFUNCTION(BlueprintCallable)
	void ClearFixedLocationList();

	UFUNCTION(BlueprintCallable)
	bool TryAddNewFixedTarget(AActor* NewTarget, ETargetFixErrorType& Error);

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

	bool bActive = false;


	float M_CachedCooldownTime;
	float M_CachedDetectionRadius;

	bool HasDependencyOnProjectile;
	bool HasProjectileMovement;
	bool HasIndependantCooltime;
	float ManaConsumption;
	bool bTargetFound;

	UPROPERTY(VisibleAnywhere, Category = "Turret|Combat")
	ATurretMeleeHitCollision* CreatedHitCollision;

	FWidgetDesciptableTurretAttribute WidgetDescriptableAttritutes;

	TWeakInterfacePtr<ISOWCharacterUIInterface> CachedUIInterface;

	TArray<ATileBase*> DetectorTiles;
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
