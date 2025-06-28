// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/Turrets/GEEC_TurretHoppyBuff.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffect.h"
#include "SOWGameplayTags.h"

struct FAttributeCapturesBuffSource {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedBase);


	FAttributeCapturesBuffSource() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedBase, Source, false);
	}
};

struct FAttributeCapturesBuffTarget {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedBase);


	FAttributeCapturesBuffTarget() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedBase, Target, false);
	}
};

static const FAttributeCapturesBuffSource& GetCapturedPropertiesBuffSource() {
	static FAttributeCapturesBuffSource AttributeCaptures;
	return AttributeCaptures;
}

static const FAttributeCapturesBuffTarget& GetCapturedPropertiesBuffTarget() {
	static FAttributeCapturesBuffTarget AttributeCaptures;
	return AttributeCaptures;
}

UGEEC_TurretHoppyBuff::UGEEC_TurretHoppyBuff()
{
	RelevantAttributesToCapture.Add(GetCapturedPropertiesBuffSource().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesBuffSource().AttackSpeedBaseDef);

	RelevantAttributesToCapture.Add(GetCapturedPropertiesBuffTarget().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesBuffTarget().AttackSpeedBaseDef);
}

void UGEEC_TurretHoppyBuff::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float AttackBuffValue, SpeedBuffValue;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesBuffSource().AttackPowerBaseDef,
		EvalParams,
		AttackBuffValue
	);

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesBuffSource().AttackSpeedBaseDef,
		EvalParams,
		SpeedBuffValue
	);
	float AdditiveRatio = Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_AdditiveBuffValue, false, 1.0f);

	AttackBuffValue *= AdditiveRatio;
	SpeedBuffValue *= AdditiveRatio;

	UE_LOG(LogTemp, Warning, TEXT("AttackPowerBase: %.2f, Speed: %.2f, Ratio: %.2f"),
		AttackBuffValue, SpeedBuffValue, AdditiveRatio);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedPropertiesBuffTarget().AttackPowerBaseDef.AttributeToCapture,
			EGameplayModOp::Additive,
			AttackBuffValue)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedPropertiesBuffTarget().AttackSpeedBaseDef.AttributeToCapture,
			EGameplayModOp::Additive,
			SpeedBuffValue)
	);
}
