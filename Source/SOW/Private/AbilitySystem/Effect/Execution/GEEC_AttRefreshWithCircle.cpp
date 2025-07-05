// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_AttRefreshWithCircle.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Interface/SOWCharacterTypeInterface.h"

#include "SOWStructTypes.h"

struct FRefreshAttributeCaptures {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedBase);

	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedRatio);


	FRefreshAttributeCaptures() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedBase, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerRatio, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerRatio, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthRatio, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedRatio, Source, false);
	}
};

static const FRefreshAttributeCaptures& GetCapturedCircleData() {
	static FRefreshAttributeCaptures AttributeCaptures;
	return AttributeCaptures;
}

UGEEC_AttRefreshWithCircle::UGEEC_AttRefreshWithCircle()
{
	RelevantAttributesToCapture.Add(GetCapturedCircleData().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().DefensePowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().MaxHealthBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().AttackSpeedBaseDef);

	RelevantAttributesToCapture.Add(GetCapturedCircleData().AttackPowerRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().DefensePowerRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().MaxHealthRatioDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().AttackSpeedRatioDef);

}

void UGEEC_AttRefreshWithCircle::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	

	AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	if (!SourceActor) {
		UE_LOG(LogTemp, Error, TEXT("The Actor in not Turret or The Actor don't have valid ASC."));
		return;
	}
	
	
	ASOWCharacterTurretBase* SourceTurret = Cast<ASOWCharacterTurretBase>(SourceActor);
	if (!SourceTurret) return;

	USOWTurretCombatComponent* SourceTurretComp = SourceTurret->FindComponentByClass<USOWTurretCombatComponent>();
	if (!SourceTurretComp) return;
	
	int32 TargetCircleCount = SourceTurret->GetCircleCount();

	FWidgetDesciptableTurretAttribute Data;
	
	/* For MaxHealth Refresh */
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().MaxHealthBaseDef,
		EvalParams,
		Data.MaxHealthBaseValue
	);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().MaxHealthRatioDef,
		EvalParams,
		Data.MaxHealthRatioValue
	);
	Data.MaxHealthRatioValue *= TargetCircleCount;

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedCircleData().MaxHealthBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			Data.MaxHealthBaseValue + Data.MaxHealthRatioValue)
	);


	/* For AttackPower Refresh */

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().AttackPowerBaseDef,
		EvalParams,
		Data.AttackPowerBaseValue
	);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().AttackPowerRatioDef,
		EvalParams,
		Data.AttackPowerRatioValue
	);
	Data.AttackPowerRatioValue *= TargetCircleCount;
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedCircleData().AttackPowerBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			Data.AttackPowerBaseValue + Data.AttackPowerRatioValue)
	);


	/* For DefencePower Refresh */
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().DefensePowerBaseDef,
		EvalParams,
		Data.DefensePowerBaseValue
	);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().DefensePowerRatioDef,
		EvalParams,
		Data.DefensePowerRatioValue
	);
	Data.DefensePowerRatioValue *= TargetCircleCount;
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedCircleData().DefensePowerBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			Data.DefensePowerBaseValue + Data.DefensePowerRatioValue)
	);


	/* For AttackSpeed Refresh */
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().AttackSpeedBaseDef,
		EvalParams,
		Data.AttackSpeedBaseValue
	);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().AttackSpeedRatioDef,
		EvalParams,
		Data.AttackSpeedRatioValue
	);
	Data.AttackSpeedRatioValue *= TargetCircleCount;
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedCircleData().AttackSpeedBaseDef.AttributeToCapture,
			EGameplayModOp::Override,
			Data.AttackSpeedBaseValue + Data.AttackSpeedRatioValue)
	);

	SourceTurretComp->SetWidgetDecriptableAttributes(Data);
	
}