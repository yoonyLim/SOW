// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TurretSynergyManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SpecialTurretManager.h"


bool UTurretSynergyManager::CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion)
{
	//SynergyContidion.GetKeys();
	ETurretRarity TargetRarity = SynergyContidion.SynergyConditionRarity;
	int TargetCount = SynergyContidion.SynergyConditionCount;

	UE_LOG(LogTemp, Warning, TEXT("Rarity : %s, TargetCount : %s"), *USOWBlueprintFunctionLibrary::EnumToFName<ETurretRarity>(TargetRarity).ToString(), *FString::FromInt(TargetCount));

	return TargetCount == 0 || (Monitor.Contains(TargetRarity) && Monitor[TargetRarity] >= TargetCount);
}

void UTurretSynergyManager::UpdateSynergyTagContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd)
{
	if (bAdd) {
		ETurretRarity InTurretRarity = InTurret->GetTurretCombatComponent()->GetTurretRarity();

		TMap<ETurretRarity, int>& CurrentMonitor = SynergyRarityMonitor[ElementType];
		if (!CurrentMonitor.Contains(InTurretRarity)) {
			CurrentMonitor.Add(InTurretRarity, 1);
		}
		else {
			int count = CurrentMonitor[InTurretRarity] + 1;
			CurrentMonitor.Add(InTurretRarity, count);
		}
		
	}

	else {
		ETurretRarity InTurretRarity = InTurret->GetTurretCombatComponent()->GetTurretRarity();

		TMap<ETurretRarity, int>& CurrentMonitor = SynergyRarityMonitor[ElementType];
		int count = CurrentMonitor[InTurretRarity] - 1;

		if (count == 0) {
			CurrentMonitor.Remove(InTurretRarity);
		}
		else {
			CurrentMonitor.Add(InTurretRarity, count);
		}
	}
	
}

void UTurretSynergyManager::AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	MonitoringTurrets.AddUnique(SummonedTurret);

	UpdateSynergyTagContainer(SummonedTurret, ElementType, true);

	int SynergyTurretCount = MonitoringTurrets.Num();
	UE_LOG(LogTemp, Warning, TEXT("SynergyTurretCount : %s"), *FString::FromInt(SynergyTurretCount));


	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
		if (!Turret) continue;

		for (int i = 1; i <= SynergyTurretCount && i < SynergyTagItems.Num(); i++) {
			//const FGameplayTag InTag = TagItem.SynergyTag;
			const FGameplayTag InTag = SynergyTagItems[i].SynergyTag;
			if (!InTag.IsValid()) continue;
			//const FSynergyCondition InCondition = TagItem.SynergyCondition;
			const FSynergyCondition InCondition = SynergyTagItems[i].SynergyCondition;
			//if (InCondition.SynergyConditionCount == 0) continue;

			
			if (USOWBlueprintFunctionLibrary::DoesActorHasTag(Turret, InTag)) continue;
			UE_LOG(LogTemp, Warning, TEXT("%s not have the tag"), *Turret->GetActorNameOrLabel());
			
			if (!CheckRarityCondition(SynergyRarityMonitor[ElementType], InCondition)) continue;
			UE_LOG(LogTemp, Warning, TEXT("%s meet the condition"), *Turret->GetActorNameOrLabel());

			USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
			TurretASC->AddLooseGameplayTag(InTag);
			UE_LOG(LogTemp, Warning, TEXT("%s got tag"), *Turret->GetActorNameOrLabel());
		}
	}
}

void UTurretSynergyManager::RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	int SynergyTurretCount = MonitoringTurrets.Num();

	UpdateSynergyTagContainer(SummonedTurret, ElementType, false);
	MonitoringTurrets.Remove(SummonedTurret);

	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
		if (!Turret) continue;
		//UE_LOG(LogTemp, Warning, TEXT("Tagging Target : %s"), *Turret->GetActorNameOrLabel());

		int i = 1;
		while (i < SynergyTurretCount && i < SynergyTagItems.Num()) {
			const FGameplayTag InTag = SynergyTagItems[i].SynergyTag;
			if (!InTag.IsValid()) {i++; continue;}
			const FSynergyCondition InCondition = SynergyTagItems[i].SynergyCondition;
			//if (InCondition.SynergyConditionCount == 0) continue;

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
}

void UTurretSynergyManager::Initialize() {
	SynergyMonitor.Add(EElementalType::Nature);
	SynergyMonitor.Add(EElementalType::Electro);
	SynergyMonitor.Add(EElementalType::Ice);

	SynergyTagContainer.Add(EElementalType::Nature);
	SynergyTagContainer.Add(EElementalType::Electro);
	SynergyTagContainer.Add(EElementalType::Ice);

	SynergyRarityMonitor.Add(EElementalType::Nature);
	SynergyRarityMonitor.Add(EElementalType::Electro);
	SynergyRarityMonitor.Add(EElementalType::Ice);

	FString CharacterDataPath = TEXT("DataTable'/Game/01Blueprints/DataTable/TurretSynergyTagData.TurretSynergyTagData'");
	SynergyTagData = LoadObject<UDataTable>(nullptr, *CharacterDataPath);
	
	checkf(SynergyTagData, TEXT("%s is not valid directory"), *CharacterDataPath);


	GlacioTurretManager = NewObject<USpecialTurretManager>(this);
	if (GlacioTurretManager)
	{
		GlacioTurretManager->Initialize();
	}
	//// etc
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
}