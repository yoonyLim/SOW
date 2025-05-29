// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SOWBlueprintFunctionLibrary.generated.h"

class USOWAbilitySystemComponent;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class SOW_API USOWBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	static USOWAbilitySystemComponent* NativeGetSOWAbilitySystemComponentFromActorInfo(AActor* InActor);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	static USOWAbilitySystemComponent* GetSOWAbilitySystemComponentFromActorInfo(AActor* InActor);

	static bool NativeDoesActorHasTag(AActor* InActor, FGameplayTag InActorTag);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	static bool DoesActorHasTag(AActor* InActor, FGameplayTag InActorTag);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static bool GetMouseWorldLocation(UObject* WorldContextObject, FVector& OutWorldLocation);

};
