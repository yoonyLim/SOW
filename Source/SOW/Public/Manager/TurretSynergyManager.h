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
class ASOWCharacterTurretSpecialBase;
class ASynergyUpdateAnnouncer;
class UCoreRuneManager;
class ASOWCharacterCoreRune;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSynergyUpdatedDelegate, int, SynergyCount, TArray<FName>, SynergyComposer);
/**
 * 
 */
UCLASS()
class SOW_API UTurretSynergyManager : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USpecialTurretManager> GlacioTurretManager;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCoreRuneManager> CoreRuneManager;

	TMap<EGlacioStatType, float> AffectStatBuffer;
	TArray<EGlacioStatType> AffectStatBuffetArr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret|Synergy")
	UDataTable* SynergyTagData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Synergy")
	ASynergyUpdateAnnouncer* SynergyUpdateAnnouncer;

	UFUNCTION(BlueprintCallable)
	void RequestToUpdateGlacioAffectedStat(ASOWCharacterTurretBase* SummonedTurret, bool OnAdd);
private:

	TMap<EElementalType, TArray<ASOWCharacterTurretBase*>> SynergyMonitor;
	TMap <EElementalType, TMap<ETurretRarity, int>> SynergyRarityMonitor;

	TMap<EElementalType, FGameplayTagContainer> SynergyTagContainer;
	TMap<EElementalType, TMap<FName, int>> SynergyTypeMonitor;

	
	// 얼음 시너지 2개 이상 추가 시, 여기로 터렛 생성 델리게이트를 전송합니다.
	
	// etc...

	bool CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion);

	void UpdateSynergyTagContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd);

	void UpdateTurretTypeContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd);


	void GrantSynergyTagToMonitoringTurrets(ASOWCharacter* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType);

	void RemoveSynergyTagFromMonitoringTurrets(ASOWCharacter* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType);

	void AnnounceSynergyUpdate(EElementalType ElementType);
public :
	/*UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyUpdatedDelegate OnSynergyUpdated;*/


	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	void AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType);

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	void RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType);

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	int GetActiveSynergyCount(EElementalType ElementType);
	// Get Active Synergy Tag Count. If you want to know how many synergy are active, use this function in bp or cpp.

	void Initialize(UDataTable* InSynergyDataTable, TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioTurret);

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	ASynergyUpdateAnnouncer* GenerateSynergyUpdateAnnouncer();

	UFUNCTION(BlueprintCallable, category = "Turret|Glacio")
	void InsertAffectStatInBuffer(EGlacioStatType statType, float value);

	UFUNCTION(BlueprintCallable, category = "Turret|Glacio")
	void RetreiveAttectStat();

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	TArray<FName> GetSynergyConfigurationTurrets(EElementalType ElementType);

	UFUNCTION(BlueprintCallable, category = "Turret|Synergy")
	int GetMonitoringTurretsCount(EElementalType ElementType);

	void SendRuneReference(ASOWCharacterCoreRune* InRune);

	ASOWCharacterTurretBase* GetGlacioInstance() const;
	ASOWCharacterCoreRune* GetCoreRuneInstance() const;

	
};
