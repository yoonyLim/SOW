// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TurretSynergyManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Turrets/SOWCharacterTurretSpecialBase.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SpecialTurretManager.h"
#include "Manager/Actor/SynergyUpdateAnnouncer.h"
#include "Characters/CoreRune/SOWCharacterCoreRune.h"
#include "Characters/CoreRune/CoreRuneManager.h"



bool UTurretSynergyManager::CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion)
{
	// 시너지 등급 조건을 확인합니다. 조건 충족 시 시너지 태그의 추가, 유지가 결정되며 미충족 시 제거가 결정됩니다.

	ETurretRarity TargetRarity = SynergyContidion.SynergyConditionRarity;
	int TargetCount = SynergyContidion.SynergyConditionCount;

	return TargetCount == 0 || (Monitor.Contains(TargetRarity) && Monitor[TargetRarity] >= TargetCount);
}

void UTurretSynergyManager::UpdateSynergyTagContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd)
{
	// 시너지의 등급 조건을 갱신합니다. 
	// bAdd = true : 조건 추가
	// bAdd = false : 조건 제거

	// 중복 방지를 고려하여 수정이 필요합니다.

	ETurretRarity InTurretRarity = InTurret->GetTurretCombatComponent()->GetTurretRarity();
	FName TurretName = InTurret->GetTurretName();

	if (!SynergyRarityMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("%s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
		return;
	}

	

	TMap<ETurretRarity, int>& CurrentMonitor = SynergyRarityMonitor[ElementType];
	if (bAdd && !SynergyTypeMonitor[ElementType].Contains(TurretName)) {
		if (!CurrentMonitor.Contains(InTurretRarity)) {
			CurrentMonitor.Add(InTurretRarity, 1);
		}
		else {
			int count = CurrentMonitor[InTurretRarity] + 1;
			CurrentMonitor.Add(InTurretRarity, count);
		}
		
	}
	else if(!bAdd && SynergyTypeMonitor[ElementType].Contains(TurretName)){
		if (!CurrentMonitor.Contains(InTurretRarity)) return;
		int count = CurrentMonitor[InTurretRarity] - 1;

		if (count == 0) {
			CurrentMonitor.Remove(InTurretRarity);
		}
		else {
			CurrentMonitor.Add(InTurretRarity, count);
		}
	}
	
}

void UTurretSynergyManager::UpdateTurretTypeContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd)
{
	if (!SynergyTypeMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("%s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
		return;
	}

	TMap<FName, int>& CurrentMonitor = SynergyTypeMonitor[ElementType];
	if (!IsValid(InTurret)) return;

	FName TurretName = InTurret->GetTurretName();
	if (bAdd) {
		if (!CurrentMonitor.Contains(TurretName)) {
			CurrentMonitor.Add(TurretName, 1);
		}
		else {
			int count = CurrentMonitor[TurretName] + 1;
			CurrentMonitor.Add(TurretName, count);
		}

	}
	else {
		if (!CurrentMonitor.Contains(TurretName)) return;
		int count = CurrentMonitor[TurretName] - 1;

		if (count == 0) {
			CurrentMonitor.Remove(TurretName);
		}
		else {
			CurrentMonitor.Add(TurretName, count);
		}
	}
}

void UTurretSynergyManager::GrantSynergyTagToMonitoringTurrets(ASOWCharacter* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType)
{
	if (!IsValid(Turret)) return;

	// 현재 활성화된 터렛 개수를 바탕으로 활성화 가능한 모든 시너지 검사
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

void UTurretSynergyManager::RemoveSynergyTagFromMonitoringTurrets(ASOWCharacter* Turret, int SynergyTurretCount, const TArray<FTurretSynergyTagItem> SynergyTagItems, EElementalType ElementType)
{
	if (!IsValid(Turret)) return;

	int i = 1;

	// 기존 시너지의 등급 조건을 따져서 제거 여부 확인
	while (i <= SynergyTurretCount && i < SynergyTagItems.Num()) {
		const FGameplayTag InTag = SynergyTagItems[i].SynergyTag;
		if (!InTag.IsValid()) { i++; continue; }
		const FSynergyCondition InCondition = SynergyTagItems[i].SynergyCondition;

		if (!USOWBlueprintFunctionLibrary::DoesActorHasTag(Turret, InTag)) { i++; continue; }
		if (CheckRarityCondition(SynergyRarityMonitor[ElementType], InCondition)) { i++; continue; }

		USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
		TurretASC->RemoveLooseGameplayTag(InTag);
		i++;
	}

	// 마지막 시너지 태그는 반드시 제거해야 하므로 루프에서 추출
	if (i < SynergyTagItems.Num() && SynergyTagItems[i].SynergyTag.IsValid()) {
		USOWAbilitySystemComponent* TurretASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Turret);
		TurretASC->RemoveLooseGameplayTag(SynergyTagItems[i].SynergyTag);
	}
}

void UTurretSynergyManager::AnnounceSynergyUpdate(EElementalType ElementType)
{
	if (SynergyUpdateAnnouncer && SynergyUpdateAnnouncer->OnSynergyUpdated.IsBound() && SynergyTypeMonitor.Contains(ElementType)) {
		int SynergyCount = GetActiveSynergyCount(ElementType);
		TArray<FName> TurretNames;
		SynergyTypeMonitor[ElementType].GetKeys(TurretNames);

		SynergyUpdateAnnouncer->OnSynergyUpdated.Broadcast(ElementType, SynergyCount, TurretNames);
	}
}

void UTurretSynergyManager::AddNewTurretDataForSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	// 특정 속성의 시너지 효과를 추가합니다.

	if (!IsValid(SummonedTurret) || !SummonedTurret->GetTurretCombatComponent()) return;
	if (!SynergyMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("%s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
		return;
	} 

	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	MonitoringTurrets.AddUnique(SummonedTurret);

	
	UpdateSynergyTagContainer(SummonedTurret, ElementType, true);
	UpdateTurretTypeContainer(SummonedTurret, ElementType, true);

	int SynergyTurretCount = SynergyTypeMonitor[ElementType].Num();
	UE_LOG(LogTemp, Warning, TEXT("SynergyTurretCount : %s"), *FString::FromInt(SynergyTurretCount));


	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	if (!SynergyDataRow) return;
	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
		GrantSynergyTagToMonitoringTurrets(Turret, SynergyTurretCount, SynergyTagItems, ElementType);
	}

	// Glacio 전용 코드
	// Glacio가 있다면 즉시 반영, 없다면 버퍼에 저장하는 용도입니다.
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
	
	else if (ElementType == EElementalType::Nature) {
		if (ASOWCharacterCoreRune* Rune = GetCoreRuneInstance()) {
			GrantSynergyTagToMonitoringTurrets(Rune, SynergyTurretCount, SynergyTagItems, ElementType);
		}
	}
	AnnounceSynergyUpdate(ElementType);
	
}

void UTurretSynergyManager::RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	// 특정 속성의 시너지 효과를 제거합니다.

	if (!IsValid(SummonedTurret) || !SummonedTurret->GetTurretCombatComponent()) return;
	if (!SynergyMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("%s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
		return;
	}

	TArray<ASOWCharacterTurretBase*>& MonitoringTurrets = SynergyMonitor[ElementType];
	
	UpdateSynergyTagContainer(SummonedTurret, ElementType, false);
	UpdateTurretTypeContainer(SummonedTurret, ElementType, false);

	int SynergyTurretCount = SynergyTypeMonitor[ElementType].Num();

	MonitoringTurrets.Remove(SummonedTurret);

	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (ASOWCharacterTurretBase* Turret : MonitoringTurrets) {
		RemoveSynergyTagFromMonitoringTurrets(Turret, SynergyTurretCount, SynergyTagItems, ElementType);
	}

	// Glacio 전용 코드
	if (ElementType == EElementalType::Ice) {
		GlacioTurretManager->OnSynergyChanged.Broadcast(SynergyTurretCount);

		GlacioTurretManager->OnTurretDead.Broadcast(
			SummonedTurret->GetTurretCombatComponent()->GetAffectStatType(),
			-SummonedTurret->GetTurretCombatComponent()->GetAffectStatValue());

		if (ASOWCharacterTurretBase* Glacio = GetGlacioInstance()) {
			RemoveSynergyTagFromMonitoringTurrets(Glacio, SynergyTurretCount, SynergyTagItems, ElementType);
		}
	}
	else if (ElementType == EElementalType::Nature) {
		if (ASOWCharacterCoreRune* Rune = GetCoreRuneInstance()) {
			RemoveSynergyTagFromMonitoringTurrets(Rune, SynergyTurretCount, SynergyTagItems, ElementType);
		}
	}
	AnnounceSynergyUpdate(ElementType);
	
}

int UTurretSynergyManager::GetActiveSynergyCount(EElementalType ElementType)
{
	// 특정 속성에 대해 활성화 중인 시너지 태그의 개수를 반환합니다.
	// 만약 부적절한 접근 시도 및 데이터 누락 시 -1을 반환합니다.

	int SynergyCount = 0;

	if (!SynergyTypeMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("%s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
		return -1;
	}

	TArray<FName> MonitoringTurrets; //= SynergyTypeMonitor[ElementType];
	SynergyTypeMonitor[ElementType].GetKeys(MonitoringTurrets);

	const FName ElementName = USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType);
	const FTurretSynergyTagData* SynergyDataRow = SynergyTagData->FindRow<FTurretSynergyTagData>(ElementName, TEXT(""));

	if (!SynergyDataRow) return -1;
	const TArray<FTurretSynergyTagItem> SynergyTagItems = SynergyDataRow->SynergyTagItems;


	for (int i = 1; i <= MonitoringTurrets.Num() && i < SynergyTagItems.Num(); i++) {

		const FGameplayTag InTag = SynergyTagItems[i].SynergyTag;
		if (!InTag.IsValid()) continue;

		const FSynergyCondition InCondition = SynergyTagItems[i].SynergyCondition;
		if (!CheckRarityCondition(SynergyRarityMonitor[ElementType], InCondition)) continue;

		SynergyCount++;
	}

	return SynergyCount;
}


TArray<FName> UTurretSynergyManager::GetSynergyConfigurationTurrets(EElementalType ElementType)
{
	// 특정 속성에 대해 시너지를 활성화시키고 있는 터렛의 종류를 FName으로 반환합니다.

	if (!SynergyTypeMonitor.Contains(ElementType)) return TArray<FName>();

	TArray<FName> OutKeys;
	SynergyTypeMonitor[ElementType].GetKeys(OutKeys);

	return OutKeys;
}

int UTurretSynergyManager::GetMonitoringTurretsCount(EElementalType ElementType)
{
	if (!SynergyMonitor.Contains(ElementType)) return -1;

	return SynergyMonitor[ElementType].Num();
}

void UTurretSynergyManager::SendRuneReference(ASOWCharacterCoreRune* InRune)
{
	CoreRuneManager->MakeCoreRuneReference(InRune);
}


void UTurretSynergyManager::Initialize(UDataTable* InSynergyDataTable, TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioTurret) {
	// 시너지를 부여받는 터렛을 속성에 맞게 모니터링하는 컨테이너
	SynergyMonitor.Add(EElementalType::Nature);
	SynergyMonitor.Add(EElementalType::Electro);
	SynergyMonitor.Add(EElementalType::Ice);

	// 시너지를 등급 조건을 확인하기 위한 컨테이너
	SynergyRarityMonitor.Add(EElementalType::Nature);
	SynergyRarityMonitor.Add(EElementalType::Electro);
	SynergyRarityMonitor.Add(EElementalType::Ice);

	// 시너지를 활설화 시키고 있는 터렛 종류를 위한 컨테이너
	SynergyTypeMonitor.Add(EElementalType::Nature);
	SynergyTypeMonitor.Add(EElementalType::Electro);
	SynergyTypeMonitor.Add(EElementalType::Ice);
	
	GenerateSynergyUpdateAnnouncer();
	SynergyTagData = InSynergyDataTable;

	GlacioTurretManager = NewObject<USpecialTurretManager>(this);
	if (GlacioTurretManager)
	{
		GlacioTurretManager->Initialize(GlacioTurret);
	}

	
	CoreRuneManager = NewObject<UCoreRuneManager>(this);
	if (CoreRuneManager) {
		CoreRuneManager->Initialize();
	}
}

ASynergyUpdateAnnouncer* UTurretSynergyManager::GenerateSynergyUpdateAnnouncer()
{
	if (IsValid(SynergyUpdateAnnouncer)) return SynergyUpdateAnnouncer;

	return SynergyUpdateAnnouncer = GetWorld()->SpawnActor<ASynergyUpdateAnnouncer>();
}



void UTurretSynergyManager::InsertAffectStatInBuffer(EGlacioStatType statType, float value)
{
	// Glacio 전용 코드
	float newValue = value;
	if (AffectStatBuffer.Contains(statType)) {
		newValue += AffectStatBuffer[statType];
	}
	AffectStatBuffer.Add(statType, newValue);
}

void UTurretSynergyManager::RetreiveAttectStat()
{
	// Glacio 전용 코드
	TArray<EGlacioStatType> Stats;
	AffectStatBuffer.GetKeys(Stats);

	for (EGlacioStatType stat : Stats) {
		GlacioTurretManager->OnTurretSummoned.Broadcast(stat, AffectStatBuffer[stat]);
	}

	AffectStatBuffer.Empty();
}



ASOWCharacterTurretBase* UTurretSynergyManager::GetGlacioInstance() const
{// Glacio 전용 코드
	check(GlacioTurretManager);
	return GlacioTurretManager->GetGlacio();
}

ASOWCharacterCoreRune* UTurretSynergyManager::GetCoreRuneInstance() const
{
	check(CoreRuneManager);
	return CoreRuneManager->GetCoreRune();
}
