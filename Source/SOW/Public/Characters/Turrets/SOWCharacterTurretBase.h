// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWGameplayTags.h"
#include "SOWCharacterTurretBase.generated.h"

class UCapsuleComponent;
class USOWTurretCombatComponent;
class UWidgetComponent;


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


#pragma region NativeGetter

	float GetDetectionRangeRadius() const;										// Get Attack Radius From Attribute Set in Turret Base 
	float GetAttackCooldownTime() const;										// Get Attack Delay From Attribute Set in Turret Base
	int32 GetCircleCount() const { return CircleCount; };						// Get Circle Count when Turret Spawning Time
	FName GetTurretName() const;												// Get Turret Name (enum) to FName

#pragma endregion


#pragma region BPGetter

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Name"))
	FName BP_GetTurretName() const;												// Get Turret Name in FName at Blueprint

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetHealthRatio() const;												// Get Health Ratio From Attribute Set in Turret Base for setting UI

	UFUNCTION(BlueprintPure)
	USOWTurretCombatComponent* GetTurretCombatComponent() const;				// Get Combat Component at other classes
	
#pragma endregion


	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Turret|Properties", meta = (ExposeOnSpawn = true))
	int32 CircleCount;															// Determine additional Attribute. multiply with attribute - ratio and add in base

	
protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretCombatComponent* TurretCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* SettingWidgetComponent;
};
