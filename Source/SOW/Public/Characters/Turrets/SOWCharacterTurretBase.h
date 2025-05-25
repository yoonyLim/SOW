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

	virtual void BeginPlay();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Properties")
	ETurretRarity TurretRarity = ETurretRarity::Common;

	void TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation);

	float GetDetectionRangeRadius() const;

	float GetAttackCooldownTime() const;
protected:


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	//UCapsuleComponent* DetectionRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretCombatComponent* TurretCombatComponent;


private:
	///* Callback Function for Collision Overlap */
	//UFUNCTION()
	//void OnTargetRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComp,
	//	int32 OtherBodyIndex,
	//	bool bFromSweep,
	//	const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnTargetRangeEndOverlap(AActor* InTargetActor);
	//// Need to Change Parameters to Bind it.

};
