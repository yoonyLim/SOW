// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWGameplayTags.h"
#include "SOWCharacterTurretBase.generated.h"

class UCapsuleComponent;
class USOWTurretCombatComponent;
class USOWTurretEvolutionComponent;
class UWidgetComponent;
struct FEffectOrientedTurretAttribute;
class UDecalComponent;



/**
 * 
 */
UCLASS()
class SOW_API ASOWCharacterTurretBase : public ASOWCharacter
{
	GENERATED_BODY()
	
public:
	ASOWCharacterTurretBase();

	virtual void Tick(float DeltaTime) override;

	void TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation);

	/* Begin ISOWCharacterUIInterface implement */
	virtual USOWCharacterUIComponent* GetCharacterUIComponent() const override;
	virtual USOWTurretUIComponent* GetTurretUIComponent() const override;
	/* End ISOWCharacterUIInterface implement */

	UFUNCTION(BlueprintCallable)
	void SwitchDetectionRangeDecal(bool On);

#pragma region NativeGetter

	float GetAttackPower() const;
	float GetCurrentHealth() const;
	float GetDetectionRangeRadius() const;										// Get Attack Radius From Attribute Set in Turret Base 
	float GetAttackCooldownTime() const;										// Get Attack Delay From Attribute Set in Turret Base
	int32 GetCircleCount() const { return CircleCount; };						// Get Circle Count when Turret Spawning Time
	FName GetTurretName() const;												// Get Turret Name (enum) to FName

#pragma endregion


#pragma region BPGetter

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Name"))
	FName BP_GetTurretName() const;												// Get Turret Name in FName at Blueprint

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Detection Range"))
	float BP_GetDetectionRangeRadius() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetHealthRatio() const;												// Get Health Ratio From Attribute Set in Turret Base for setting UI

	UFUNCTION(BlueprintPure)
	USOWTurretCombatComponent* GetTurretCombatComponent() const;				// Get Combat Component at other classes
#pragma endregion

#pragma region BlueprintFunctions
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Bind On Target Dead"))
	void BP_BindOnTargetDead(AActor* Target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Deactivate Turret All Function Async"))
	void BP_DeactivateTurretAllFunctionAsync();
#pragma endregion

	



	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Turret|Properties", meta = (ExposeOnSpawn = true))
	int32 CircleCount;															// Determine additional Attribute. multiply with attribute - ratio and add in base

	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitFromDataAsset() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretCombatComponent* TurretCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretEvolutionComponent* TurretEvolutionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* SettingWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	USOWTurretUIComponent* TurretUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal")
	UDecalComponent* DetectionRangeDecal;

private:
	UFUNCTION()
	void OnGameplayEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	

	UFUNCTION()
	void OnGameplayEffectRemoved(const FActiveGameplayEffect& Effect);


	UFUNCTION()
	void OnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION(BlueprintCallable)
	void GetModifiedAttributesByGameplayEffects(FEffectOrientedTurretAttribute& BuffData, FEffectOrientedTurretAttribute& DebuffData);



	void AddBuffData(const FGameplayAttribute& ModifiedAttr, FEffectOrientedTurretAttribute& Data, float Value);

	FTimerHandle DeathTimerHandle;
};
