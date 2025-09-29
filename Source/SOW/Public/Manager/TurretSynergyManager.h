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

	// 얼음 시너지 2개 이상 추가 시, 여기로 터렛 생성 델리게이트를 전송합니다.
	USpecialTurretManager* GlacioTurretManager;
	// etc...

	bool CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion);

	void UpdateSynergyTagContainer(ETurretRarity TurretRarity, EElementalType ElementType, bool bAdd);

	void GrantSynergyTagToMonitoringTurrets(ASOWCharacterTurretBase* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType);

	void RemoveSynergyTagFromMonitoringTurrets(ASOWCharacterTurretBase* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType);
public :
	//FOnNewTurretSummonedDelegate OnNewTurretSummmoned;
	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	void AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType);

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	void RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType);

	void Initialize();

	ASOWCharacterTurretBase* GetGlacioInstance();
};
