// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SOWAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API USOWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SOW|GameplayAbility")
	bool TryActivateAbilityWithTag(FGameplayTag InAbilityTag);

	void OnAbilityInputPressed(FGameplayTag InInputTag);
	void OnAbilityInputReleased(FGameplayTag InInputTag);
};
