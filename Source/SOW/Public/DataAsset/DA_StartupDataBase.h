// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_StartupDataBase.generated.h"

class USOWGameplayAbilityBase;
class UGameplayEffect;
class USOWAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SOW_API UDA_StartupDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<USOWGameplayAbilityBase> > ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<USOWGameplayAbilityBase> > ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf< UGameplayEffect>> StartupGameplayEffects;

	void GrantAbility(const TArray<TSubclassOf<USOWGameplayAbilityBase>>& InAbilitiesToGive, USOWAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
