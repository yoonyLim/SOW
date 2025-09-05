// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_InitTurretAttribute.h"
#include "Engine/DataTable.h"
#include "SOWStructTypes.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "AbilitySystem/SOWAttributeSet.h"

struct FAttributeCapturesTurretData {


	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);

	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedBase);

	DECLARE_ATTRIBUTE_CAPTUREDEF(DetectionRange);

	FAttributeCapturesTurretData() {


		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedBase, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DetectionRange, Target, false);
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



	RelevantAttributesToCapture.Add(GetCapturedTurretData().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().AttackSpeedBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedTurretData().DetectionRangeDef);
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
			GetCapturedTurretData().AttackPowerBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->AttackPowerBase)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().AttackSpeedBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->AttackSpeedBase)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedTurretData().DetectionRangeDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->DetectionRange)
	);
}