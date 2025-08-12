// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SOWCharacterUIComponent.h"
#include "SOWTurretUIComponent.generated.h"
class ASOWCharacterTurretBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetSelectionPriorityChangedDelegate, ETurretTargetSelectionPriority, NewPriority);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickSildeButton, bool, ToLeft);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickEvolutionButton, EEvolutionType, Type);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectAppliedDelegate, FEffectOrientedTurretAttribute, EffectData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffTagAddOrRemoved);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectionRangeChangedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, FWidgetDescAtt, AttData);

/**
 * 
 */
UCLASS()
class SOW_API USOWTurretUIComponent : public USOWCharacterUIComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FOnTargetSelectionPriorityChangedDelegate OnPriorityChangedInTurret;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnClickSildeButton PriorityChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnClickEvolutionButton OnTryToEvolveWith;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEffectAppliedDelegate OnEffectApplied;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEffectAppliedDelegate OnEffectRemoved;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnBuffTagAddOrRemoved OnTagChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDetectionRangeChangedDelegate OnRangeChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttributeChanged OnAttackChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttributeChanged OnDefenseChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttributeChanged OnSpeedChanged;

	UFUNCTION(BlueprintPure)
	ASOWCharacterTurretBase* GetOwnerTurret() const;
};
