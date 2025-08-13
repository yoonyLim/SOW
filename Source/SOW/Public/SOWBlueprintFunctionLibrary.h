// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SOWEnumTypes.h"
#include "GameplayEffect.h"
#include "SOWBlueprintFunctionLibrary.generated.h"

class USOWAbilitySystemComponent;
class ASOWCharacterTurretBase;
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

	static bool SpawnTurretWithCircleCount(UObject* WorldContextObject, const TSubclassOf<ASOWCharacterTurretBase>& InTurretClass, const FVector& InTargetLoc, const FRotator& InTargetRot, const int32 InCircleCount);

	template <typename T>
	static FName EnumToFName(const T EnumValue);

	static bool IsTarget(ETurretTargetSelectionPolicy OwnerPolicy, ESOWCharacterType TargetType);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	static AActor* GetOwnerActorFromGameplayEffectSpec(const FGameplayEffectSpec& GESpec);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	static void RequestToGenerateOnTimeCurrency(UObject* WorldContextObject, const FGameplayTag& InTag, const int InCount);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	static bool QueryForCurrencyCountSufficient(UObject* WorldContextObject, const FGameplayTag& InTag, const int InCount);
};

template <typename T>
FName USOWBlueprintFunctionLibrary::EnumToFName(const T EnumValue)
{
	UEnum* EnumPtr = StaticEnum<T>();
	if (!EnumPtr) return NAME_None;

	// Enum 이름을 FName으로 반환

	FString EnumNameStr = EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue));
	// 예: "Walking"

	FName CleanFName(*EnumNameStr);

	return CleanFName;
}