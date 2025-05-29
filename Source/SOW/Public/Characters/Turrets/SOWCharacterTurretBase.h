// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWGameplayTags.h"
#include "SOWCharacterTurretBase.generated.h"

class UCapsuleComponent;
class USOWTurretCombatComponent;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Properties")
	bool bIsActivated = false;

protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretCombatComponent* TurretCombatComponent;


	UFUNCTION(BlueprintCallable)
	void ActivateTurret();


	void FollowMouseLocationWhileDeactive(float DeltaTime);
};
