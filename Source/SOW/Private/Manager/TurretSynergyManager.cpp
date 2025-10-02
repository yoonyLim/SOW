// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TurretSynergyManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Turrets/SOWCharacterTurretSpecialBase.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SpecialTurretManager.h"


bool UTurretSynergyManager::CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion)
{
	ETurretRarity TargetRarity = SynergyContidion.SynergyConditionRarity;
	int TargetCount = SynergyContidion.SynergyConditionCount;

	//UE_LOG(LogTemp, Warning, TEXT("Rarity : %s, TargetCount : %s"), *USOWBlueprintFunctionLibrary::EnumToFName<ETurretRarity>(TargetRarity).ToString(), *FString::FromInt(TargetCount));

	return TargetCount == 0 || (Monitor.Contains(TargetRarity) && Monitor[TargetRarity] >= TargetCount);
}

void UTurretSynergyManager::UpdateSynergyTagContainer(ETurretRarity TurretRarity, EElementalType ElementType, bool bAdd)
{
	TMap<ETurretRarity, int>& CurrentMonitor = SynergyRarityMonitor[ElementType];
	if (bAdd) {
		if (!CurrentMonitor.Contains(TurretRarity)) {
			CurrentMonitor.Add(TurretRarity, 1);
		}
		else {
			int count = CurrentMonitor[TurretRarity] + 1;
			CurrentMonitor.Add(TurretRarity, count);
		}
		
	}
	else {
		int count = CurrentMonitor[TurretRarity] - 1;

		if (count == 0) {
			CurrentMonitor.Remove(TurretRarity);
		}
		else {
			CurrentMonitor.Add(TurretRarity, count);
		}
	}
	
}

void UTurretSynergyManager::GrantSynergyTagToMonitoringTurrets(ASOWCharacterTurretBase* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType)
{
	if (!Turret) return;

	for (int i = 1; i <= SynergyTurretCount && i < SynergyTagItems.Num(); i++) {

		const FGameplayTag InTag = SynergyTagItems[i].SynergyTag;
		if (!InTag.IsValid()) continue;

		const FSynergyCondition InCondition = SynergyTagItems[i].SynergyCondition;

		if (USOWBlueprintFunctionLibrary::DoesActorHasTag(Turret, InTag)) continue;
		if (!CheckRarityCondition(SynergyRarityMonitor[ElementType], InCondition)) continue;

		USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
		TurretASC->AddLooseGameplayTag(InTag);
	}
}

void UTurretSynergyManager::RemoveSynergyTagFromMonitoringTurrets(ASOWCharacterTurretBase* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType)
{
	if (!Turret) return;

	int i = 1;
	while (i < SynergyTurretCount && i < SynergyTagItems.Num()) {
		const FGameplayTag InTag = SynergyTagItems[i].SynergyTag;
		if (!InTag.IsValid()) { i++; continue; }
		const FSynergyCondition InCondition = SynergyTagItems[i].SynergyCondition;

		if (!USOWBlueprintFunctionLibrary::DoesActorHasTag(Turret, InTag)) { i++; continue; }
		if (CheckRarityCondition(SynergyRarityMonitor[ElementType], InCondition)) { i++; continue; }

		USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
		TurretASC->RemoveLooseGameplayTag(InTag);
		i++;
	}
	if (i < SynergyTagItems.Num() && SynergyTagItems[i].SynergyTag.IsValid()) {
		USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
		TurretASC->RemoveLooseGameplayTag(SynergyTagItems[i].SynergyTag);
	}
}

void UTurretSynergyManager::AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	if (!SummonedTurret || !SummonedTurret->GetTurretCombatComponent()) return;

	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	MonitoringTurrets.AddUnique(SummonedTurret);

	ETurretRarity InTurretRarity = SummonedTurret->GetTurretCombatComponent()->GetTurretRarity();
	UpdateSynergyTagContainer(InTurretRarity, ElementType, true);

	int SynergyTurretCount = MonitoringTurrets.Num();
	UE_LOG(LogTemp, Warning, TEXT("SynergyTurretCount : %s"), *FString::FromInt(SynergyTurretCount));


	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	if (!SynergyDataRow) return;
	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
		GrantSynergyTagToMonitoringTurrets(Turret, SynergyTurretCount, SynergyTagItems, ElementType);
	}

	// Glacio 전용 코드
	if (ElementType == EElementalType::Ice) {
		GlacioTurretManager->OnSynergyChanged.Broadcast(SynergyTurretCount);
		if (GlacioTurretManager->OnTurretSummoned.IsBound()) {
			GlacioTurretManager->OnTurretSummoned.Broadcast(
				SummonedTurret->GetTurretCombatComponent()->GetAffectStatType(),
				SummonedTurret->GetTurretCombatComponent()->GetAffectStatValue());
		}
		else {
			InsertAffectStatInBuffer(
				SummonedTurret->GetTurretCombatComponent()->GetAffectStatType(),
				SummonedTurret->GetTurretCombatComponent()->GetAffectStatValue());
		}
		

		if (ASOWCharacterTurretBase* Glacio = GetGlacioInstance()) {
			GrantSynergyTagToMonitoringTurrets(Glacio, SynergyTurretCount, SynergyTagItems, ElementType);
		}
	}
	
}

void UTurretSynergyManager::RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	if (!SummonedTurret || !SummonedTurret->GetTurretCombatComponent()) return;

	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	int SynergyTurretCount = MonitoringTurrets.Num();

	ETurretRarity InTurretRarity = SummonedTurret->GetTurretCombatComponent()->GetTurretRarity();
	UpdateSynergyTagContainer(InTurretRarity, ElementType, true);

	MonitoringTurrets.Remove(SummonedTurret);

	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
		RemoveSynergyTagFromMonitoringTurrets(Turret, SynergyTurretCount, SynergyTagItems, ElementType);
	}

	// Glacio 전용 코드
	if (ElementType == EElementalType::Ice) {
		GlacioTurretManager->OnSynergyChanged.Broadcast(SynergyTurretCount - 1);

		GlacioTurretManager->OnTurretDead.Broadcast(
			SummonedTurret->GetTurretCombatComponent()->GetAffectStatType(),
			-SummonedTurret->GetTurretCombatComponent()->GetAffectStatValue());

		if (ASOWCharacterTurretBase* Glacio = GetGlacioInstance()) {
			RemoveSynergyTagFromMonitoringTurrets(Glacio, SynergyTurretCount, SynergyTagItems, ElementType);
		}
	}
	
}

void UTurretSynergyManager::Initialize(UDataTable* InSynergyDataTable, TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioTurret) {
	SynergyMonitor.Add(EElementalType::Nature);
	SynergyMonitor.Add(EElementalType::Electro);
	SynergyMonitor.Add(EElementalType::Ice);

	SynergyTagContainer.Add(EElementalType::Nature);
	SynergyTagContainer.Add(EElementalType::Electro);
	SynergyTagContainer.Add(EElementalType::Ice);

	SynergyRarityMonitor.Add(EElementalType::Nature);
	SynergyRarityMonitor.Add(EElementalType::Electro);
	SynergyRarityMonitor.Add(EElementalType::Ice);

	//FString CharacterDataPath = TEXT("DataTable'/Game/01Blueprints/DataTable/TurretSynergyTagData.TurretSynergyTagData'");
	
	
	//checkf(SynergyTagData, TEXT("%s is not valid directory"), *CharacterDataPath);
	SynergyTagData = InSynergyDataTable;

	GlacioTurretManager = NewObject<USpecialTurretManager>(this);
	if (GlacioTurretManager)
	{
		GlacioTurretManager->Initialize(GlacioTurret);
	}
	//// etc
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
}

void UTurretSynergyManager::InsertAffectStatInBuffer(EGlacioStatType statType, float value)
{
	int newValue = value;
	if (AffectStatBuffer.Contains(statType)) {
		newValue += AffectStatBuffer[statType];
	}
	AffectStatBuffer.Add(statType, newValue);
}

void UTurretSynergyManager::RetreiveAttectStat()
{
	TArray<EGlacioStatType> Stats;
	AffectStatBuffer.GetKeys(Stats);

	for (EGlacioStatType stat : Stats) {
		GlacioTurretManager->OnTurretSummoned.Broadcast(stat, AffectStatBuffer[stat]);
	}

	AffectStatBuffer.Empty();
}

ASOWCharacterTurretBase* UTurretSynergyManager::GetGlacioInstance()
{
	check(GlacioTurretManager);
	return GlacioTurretManager->GetGlacio();
}