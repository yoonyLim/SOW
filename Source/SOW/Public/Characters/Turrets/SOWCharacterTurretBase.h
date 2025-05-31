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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Properties")
	ETurretRarity TurretRarity = ETurretRarity::Common;

	void TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation);

	float GetDetectionRangeRadius() const;

	float GetAttackCooldownTime() const;

	FName GetTurretName() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetHealthRatio() const;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Turret|Properties", meta = (ExposeOnSpawn = true))
	int32 CircleCount;

	int32 GetCircleCount() const { return CircleCount; };
protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretCombatComponent* TurretCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

};
