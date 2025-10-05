// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"
#include "Components/ActorComponent.h"
#include "SOWTurretEvolutionComponent.generated.h"

class ASOWCharacterTurretBase;
class UGameplayEffect;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWTurretEvolutionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWTurretEvolutionComponent();

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	void SetStatusMaxLevel(int32 MaxLv) { StatusMaxLevel = MaxLv; }


	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	int32 GetTurretEvolutionPropertyLevel() const { return EvolutionPropertyLevel; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	int32 GetTurretEvolutionStatusLevel() const { return EvolutionStatusLevel; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	void GetPropertyDescriptString(FString& OutString);

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	void GetPropertyResourceString(FString& OutCurrency, FString& OutPercentage);

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	void GetStatusResourceString(FString& OutCurrency, FString& OutPercentage);

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	void GetStatusNextValueString(FString& OutAtk, FString& OutSpd);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCurveTable* PropertyResourceData;

	UPROPERTY(VisibleAnywhere)
	UCurveTable* StatusResourceData;

private:
	UPROPERTY()
	ASOWCharacterTurretBase* CachedOwnerCharacter;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> AlphaData;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> BetaData;

	UPROPERTY()
	TArray<FTurretEvolutionItem> PropertyData;

	int32 EvolutionPropertyLevel = 0;
	int32 EvolutionStatusLevel = 0;

	int32 PropertyMaxLevel = 4;
	int32 StatusMaxLevel = 1;

	int32 CurrencySpentForProp = 0;
	int32 CurrencySpentForStat = 0;

	UFUNCTION()
	void TryEvolution(EEvolutionType Type);

	bool CheckResourceAndProb(EEvolutionType Type);

	void MakeAndSendEvolutionLog();
};
