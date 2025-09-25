// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TurretSynergyManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWStructTypes.h"
#include "SOWGameplayTags.h"


void UTurretSynergyManager::AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	int SynergyTurretCount = MonitoringTurrets.Num() + 1;
	UE_LOG(LogTemp, Warning, TEXT("SynergyTurretCount : %s"), *FString::FromInt(SynergyTurretCount));


	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTag* ResistanceDataRow = SynergyTagData->FindRow<FTurretSynergyTag>(ElementName, TEXT(""));
	const TArray<FGameplayTag> SynergyTagArray = ResistanceDataRow->SynergyTag;

	FGameplayTag NewTag = SynergyTagArray[SynergyTurretCount];

	if (NewTag.IsValid()) {
		for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
			if (USOWBlueprintFunctionLibrary::DoesActorHasTag(Turret, NewTag)) return;

			USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
			TurretASC->AddLooseGameplayTag(NewTag);

			UE_LOG(LogTemp, Warning, TEXT("New Synergy Tag added to %s"), *Turret->GetActorNameOrLabel());

		}
	} 

	SynergyTagContainer[ElementType].AddTag(NewTag);
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(SummonedTurret, SynergyTagContainer[ElementType]);
	MonitoringTurrets.AddUnique(SummonedTurret);
}

void UTurretSynergyManager::RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	int SynergyTurretCount = MonitoringTurrets.Num();


	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTag* ResistanceDataRow = SynergyTagData->FindRow<FTurretSynergyTag>(ElementName, TEXT(""));
	const TArray<FGameplayTag> SynergyTagArray = ResistanceDataRow->SynergyTag;


	FGameplayTag InTag = SynergyTagArray[SynergyTurretCount];
	if (InTag.IsValid()) {
		for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
			if (!USOWBlueprintFunctionLibrary::DoesActorHasTag(Turret, InTag)) return;

			USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
			TurretASC->RemoveLooseGameplayTag(InTag);

			UE_LOG(LogTemp, Warning, TEXT("Synergy Tag removed from %s"), *Turret->GetActorNameOrLabel());
		}

		SynergyTagContainer[ElementType].RemoveTag(InTag);
	}


	MonitoringTurrets.Remove(SummonedTurret);	
	UE_LOG(LogTemp, Warning, TEXT("SynergyTurretCount : %s"), *FString::FromInt(SynergyTurretCount));
}

void UTurretSynergyManager::Initialize() {
	SynergyMonitor.Add(EElementalType::Nature);
	SynergyMonitor.Add(EElementalType::Electro);
	SynergyMonitor.Add(EElementalType::Ice);

	SynergyTagContainer.Add(EElementalType::Nature);
	SynergyTagContainer.Add(EElementalType::Electro);
	SynergyTagContainer.Add(EElementalType::Ice);

	FString CharacterDataPath = TEXT("DataTable'/Game/01Blueprints/DataTable/TurretSynergyTagData.TurretSynergyTagData'");
	SynergyTagData = LoadObject<UDataTable>(nullptr, *CharacterDataPath);
	
	checkf(SynergyTagData, TEXT("%s is not valid directory"), *CharacterDataPath);

	//// etc
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
	//SynergyMonitor.Add(EElementalType::Nature);
}