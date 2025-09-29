// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"
#include "SOWGameplayTags.h"
#include "TurretSynergyManager.generated.h"

class ASOWCharacterTurretBase;
class USpecialTurretManager;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnNewTurretSummonedDelegate, ASOWCharacterTurretBase*, SummonedTurret, EElementalType, ElementType);
/**
 * 
 */
UCLASS()
class SOW_API UTurretSynergyManager : public UObject
{
	GENERATED_BODY()
	
private:

	TMap<EElementalType, TArray<ASOWCharacterTurretBase*>> SynergyMonitor;
	TMap <EElementalType, TMap<ETurretRarity, int>> SynergyRarityMonitor;

	TMap<EElementalType, FGameplayTagContainer> SynergyTagContainer;
	UDataTable* SynergyTagData;

	USpecialTurretManager* GlacioTurretManager;
	// etc...

	bool CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion);

	void UpdateSynergyTagContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd);


public :
	//FOnNewTurretSummonedDelegate OnNewTurretSummmoned;
	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	void AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType);

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	void RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType);

	void Initialize();

};
