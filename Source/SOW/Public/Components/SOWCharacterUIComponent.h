// Fill out your copyright notice in the Description page of Project Settings.

// added by PGH
// mix-in target -> SOWCharacter
// purpose : Send implemented delegate to all UI Assets in WBP. 

// OnCurrentHealthChanged - every character has health attribute and need to listen health change

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"
#include "Components/ActorComponent.h"
#include "SOWCharacterUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthPercentChangedDelegate, float, NewPercent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetSelectionPriorityChangedDelegate, ETurretTargetSelectionPriority, NewPriority);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickSildeButton, bool, ToLeft);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickEvolutionButton, EEvolutionType, Type);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectAppliedDelegate, FEffectOrientedTurretAttribute, EffectData);

class ASOWCharacterTurretBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWCharacterUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentChangedDelegate OnCurrentHealthChanged;

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

	UFUNCTION(BlueprintPure)
	ASOWCharacterTurretBase* GetOwnerTurret() const;
};
