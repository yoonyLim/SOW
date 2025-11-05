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



void UTurretSynergyManager::RequestToUpdateGlacioAffectedStat(ASOWCharacterTurretBase* TargetTurret, bool OnAdd)
{
	if (!IsValid(TargetTurret)) return;

	if (OnAdd) {
		if (GlacioTurretManager->OnTurretSummoned.IsBound()) {
			GlacioTurretManager->OnTurretSummoned.Broadcast(
				TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
				TargetTurret->GetTurretCombatComponent()->GetAffectStatValue());
		}

		InsertAffectStatInBuffer(
			TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
			TargetTurret->GetTurretCombatComponent()->GetAffectStatValue());
	}
	else {
		if (GlacioTurretManager->OnTurretDead.IsBound()) {
			GlacioTurretManager->OnTurretDead.Broadcast(
				TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
				-TargetTurret->GetTurretCombatComponent()->GetAffectStatValue());
		}

		InsertAffectStatInBuffer(
			TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
			-TargetTurret->GetTurretCombatComponent()->GetAffectStatValue());
	}

	
}

void UTurretSynergyManager::RequestToUpdateGlacioAffectedStatConstant(ASOWCharacterTurretBase* TargetTurret, bool OnAdd, float Value)
{
	if (!IsValid(TargetTurret)) return;

	if (OnAdd) {
		if (GlacioTurretManager->OnTurretSummoned.IsBound()) {
			GlacioTurretManager->OnTurretSummoned.Broadcast(
				TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
				Value);
		}

		InsertAffectStatInBuffer(
			TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
			Value);
	}
	else {
		if (GlacioTurretManager->OnTurretDead.IsBound()) {
			GlacioTurretManager->OnTurretDead.Broadcast(
				TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
				-Value);
		}

		InsertAffectStatInBuffer(
			TargetTurret->GetTurretCombatComponent()->GetAffectStatType(),
			-Value);
	}
}

bool UTurretSynergyManager::CheckRarityCondition(const TMap<ETurretRarity, int> Monitor, const FSynergyCondition& SynergyContidion)
{
	// �ó��� ��� ������ Ȯ���մϴ�. ���� ���� �� �ó��� �±��� �߰�, ������ �����Ǹ� ������ �� ���Ű� �����˴ϴ�.

	ETurretRarity TargetRarity = SynergyContidion.SynergyConditionRarity;
	int TargetCount = SynergyContidion.SynergyConditionCount;

	return TargetCount == 0 || (Monitor.Contains(TargetRarity) && Monitor[TargetRarity] >= TargetCount);
}

void UTurretSynergyManager::UpdateSynergyTagContainer(ASOWCharacterTurretBase* InTurret, EElementalType ElementType, bool bAdd)
{
	// �ó����� ��� ������ �����մϴ�. 
	// bAdd = true : ���� �߰�
	// bAdd = false : ���� ����

	// �ߺ� ������ �����Ͽ� ������ �ʿ��մϴ�.

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

	// ���� Ȱ��ȭ�� �ͷ� ������ �������� Ȱ��ȭ ������ ��� �ó��� �˻�
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

	// ���� �ó����� ��� ������ ������ ���� ���� Ȯ��
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

	// ������ �ó��� �±״� �ݵ�� �����ؾ� �ϹǷ� �������� ����
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
	// Ư�� �Ӽ��� �ó��� ȿ���� �߰��մϴ�.

	if (!IsValid(SummonedTurret) || !SummonedTurret->GetTurretCombatComponent()) return;
	if (!SynergyMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("Add synergy / %s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
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

	// Glacio ���� �ڵ�
	// Glacio�� �ִٸ� ��� �ݿ�, ���ٸ� ���ۿ� �����ϴ� �뵵�Դϴ�.
	if (ElementType == EElementalType::Ice) {
		GlacioTurretManager->OnSynergyChanged.Broadcast(SynergyTurretCount);

		RequestToUpdateGlacioAffectedStat(SummonedTurret, true);
		

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

	UE_LOG(LogTemp, Warning, TEXT("Add Synergy Result"));
	UE_LOG(LogTemp, Warning, TEXT("------ Composer ------"));
	for (FName name : GetSynergyConfigurationTurrets(ElementType)) {
		UE_LOG(LogTemp, Warning, TEXT(": %s"), *name.ToString());
	}
	
}

void UTurretSynergyManager::RemoveTurratDataFromSynergy(ASOWCharacterTurretBase* SummonedTurret, EElementalType ElementType)
{
	// Ư�� �Ӽ��� �ó��� ȿ���� �����մϴ�.

	if (!IsValid(SummonedTurret) || !SummonedTurret->GetTurretCombatComponent()) return;
	if (!SynergyMonitor.Contains(ElementType)) {
		UE_LOG(LogTemp, Error, TEXT("Remove Synergy / %s is not implemented synergy element."), *USOWBlueprintFunctionLibrary::EnumToFName<EElementalType>(ElementType).ToString());
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

	// Glacio ���� �ڵ�
	// Glacio ��ȯ ������ ���� ���� ��� Glacio�� �����մϴ�.
	if (ElementType == EElementalType::Ice) {
		GlacioTurretManager->OnSynergyChanged.Broadcast(SynergyTurretCount);

		/*if (GlacioTurretManager->OnTurretDead.IsBound()) {
			GlacioTurretManager->OnTurretDead.Broadcast(
				SummonedTurret->GetTurretCombatComponent()->GetAffectStatType(),
				-SummonedTurret->GetTurretCombatComponent()->GetAffectStatValue());
		}

		InsertAffectStatInBuffer(
			SummonedTurret->GetTurretCombatComponent()->GetAffectStatType(),
			-SummonedTurret->GetTurretCombatComponent()->GetAffectStatValue());*/
		RequestToUpdateGlacioAffectedStat(SummonedTurret, false);

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

	UE_LOG(LogTemp, Warning, TEXT("Remove Synergy Result"));
	UE_LOG(LogTemp, Warning, TEXT("------ Composer ------"));
	for (FName name : GetSynergyConfigurationTurrets(ElementType)) {
		UE_LOG(LogTemp, Warning, TEXT(": %s"), *name.ToString());
	}
	
}

int UTurretSynergyManager::GetActiveSynergyCount(EElementalType ElementType)
{
	// Ư�� �Ӽ��� ���� Ȱ��ȭ ���� �ó��� �±��� ������ ��ȯ�մϴ�.
	// ���� �������� ���� �õ� �� ������ ���� �� -1�� ��ȯ�մϴ�.

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
	// Ư�� �Ӽ��� ���� �ó����� Ȱ��ȭ��Ű�� �ִ� �ͷ��� ������ FName���� ��ȯ�մϴ�.

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

	// �ó����� �ο��޴� �ͷ��� �Ӽ��� �°� ����͸��ϴ� �����̳�
	SynergyMonitor.Add(EElementalType::Nature);
	SynergyMonitor.Add(EElementalType::Electro);
	SynergyMonitor.Add(EElementalType::Ice);
	SynergyMonitor.Add(EElementalType::Flame);

	// �ó����� ��� ������ Ȯ���ϱ� ���� �����̳�
	SynergyRarityMonitor.Add(EElementalType::Nature);
	SynergyRarityMonitor.Add(EElementalType::Electro);
	SynergyRarityMonitor.Add(EElementalType::Ice);
	SynergyRarityMonitor.Add(EElementalType::Flame);

	// �ó����� Ȱ��ȭ ��Ű�� �ִ� �ͷ� ������ ���� �����̳�
	SynergyTypeMonitor.Add(EElementalType::Nature);
	SynergyTypeMonitor.Add(EElementalType::Electro);
	SynergyTypeMonitor.Add(EElementalType::Ice);
	SynergyTypeMonitor.Add(EElementalType::Flame);
	
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
	// Glacio ���� �ڵ�
	float newValue = value;
	if (AffectStatBuffer.Contains(statType)) {
		newValue += AffectStatBuffer[statType];
	}
	AffectStatBuffer.Add(statType, newValue);
}

void UTurretSynergyManager::RetreiveAttectStat()
{
	// Glacio ���� �ڵ�
	TArray<EGlacioStatType> Stats;
	AffectStatBuffer.GetKeys(Stats);

	for (EGlacioStatType stat : Stats) {
		GlacioTurretManager->OnTurretSummoned.Broadcast(stat, AffectStatBuffer[stat]);
	}

	//AffectStatBuffer.Empty();
}



ASOWCharacterTurretBase* UTurretSynergyManager::GetGlacioInstance() const
{// Glacio ���� �ڵ�
	check(GlacioTurretManager);
	return GlacioTurretManager->GetGlacio();
}

ASOWCharacterCoreRune* UTurretSynergyManager::GetCoreRuneInstance() const
{
	check(CoreRuneManager);
	return CoreRuneManager->GetCoreRune();
}
