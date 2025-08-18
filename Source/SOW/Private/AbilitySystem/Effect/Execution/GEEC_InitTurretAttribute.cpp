// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_InitTurretAttribute.h"
#include "Engine/DataTable.h"
#include "SOWStructTypes.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "AbilitySystem/SOWAttributeSet.h"

struct FAttributeCapturesTurretData {
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DetectionRange);

	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxManaBase);

	FAttributeCapturesTurretData() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthRatio, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerRatio, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerRatio, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedRatio, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DetectionRange, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxManaBase, Target, false);
	}
};


static const FAttributeCapturesTurretData& GetCapturedTurretData() {
	static FAttributeCapturesTurretData AttributeCaptures;
	return AttributeCaptures;
}

UGEEC_InitTurretAttribute::UGEEC_InitTurretAttribute()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/01Blueprints/DataTable/TurretAttributeData.TurretAttributeData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(*CharacterDataPath);
	if (DT_MYCHARACTER.Succeeded()) {
		AttributeData = DT_MYCHARACTER.Object;
	}
	else {
		AttributeData = nullptr;
	}

	RelevantAttributesToCapture.Add(GetCapturedTurretData().MaxHealthBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().MaxHealthRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().DefensePowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().DefensePowerRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().AttackPowerRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().AttackSpeedBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().AttackSpeedRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().DetectionRangeDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().MaxManaBaseDef);
}

void UGEEC_InitTurretAttribute::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	if (!ExecutionParams.GetSourceAbilitySystemComponent()) {
		return;
	} 
	
	AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();

	if (!SourceActor) {
		UE_LOG(LogTemp, Error, TEXT("The Actor doesn't have ASC!"));
		return;
	}
	ASOWCharacterTurretBase* SourceTurret = Cast<ASOWCharacterTurretBase>(SourceActor);

	if (!SourceTurret) {
		UE_LOG(LogTemp, Error, TEXT("The Actor is not a turret!"));
		return;
	}

	FName TurretName = SourceTurret->GetTurretName();
	UE_LOG(LogTemp, Error, TEXT("Turret : %s"), * TurretName.ToString());
	FTurretAttributeData* ResistanceDataRow = AttributeData->FindRow<FTurretAttributeData>(TurretName, TEXT(""));

	if (!ResistanceDataRow) {
		//UE_LOG(LogTemp, Error, TEXT("[ %s ] is not exist Turret Name. Please use another name or add name."), TurretName);
		return;
	}

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().MaxHealthBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->MaxHealthBase)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().MaxHealthRatioDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->MaxHealthRatio)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().DefensePowerBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->DefensePowerBase)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().DefensePowerRatioDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->DefensePowerRatio)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().AttackPowerBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->AttackPowerBase)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().AttackPowerRatioDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->AttackPowerRatio)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().AttackSpeedBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->AttackSpeedBase)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().AttackSpeedRatioDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->AttackSpeedRatio)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().DetectionRangeDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->DetectionRange)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().MaxManaBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->MaxManaBase)
	);


	SourceTurret->SetManaConsumption(ResistanceDataRow->ManaConsumption);
	UE_LOG(LogTemp, Warning, TEXT("Mana Consumption : %s"), *FString::SanitizeFloat(ResistanceDataRow->ManaConsumption));
}