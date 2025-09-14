// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_CalculatePercentageDamage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffect.h"
#include "SOWGameplayTags.h"


struct FAttributeCapturesPercentageDamage {
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);


	FAttributeCapturesPercentageDamage() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, CurrentHealth, Target, false);
	}
};

static const FAttributeCapturesPercentageDamage& GetCapturedPropertiesPercentageDamage() {
	static FAttributeCapturesPercentageDamage AttributeCaptures;
	return AttributeCaptures;
}



UGEEC_CalculatePercentageDamage::UGEEC_CalculatePercentageDamage()
{
	RelevantAttributesToCapture.Add(GetCapturedPropertiesPercentageDamage().MaxHealthBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesPercentageDamage().DefensePowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesPercentageDamage().CurrentHealthDef);

}

void UGEEC_CalculatePercentageDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();


	float TargetMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesPercentageDamage().MaxHealthBaseDef,
		EvalParams,
		TargetMaxHealth
	);


	// Base Defense Power
	float L_DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesPercentageDamage().DefensePowerBaseDef,
		EvalParams,
		L_DefensePower
	);

	float Percentage = Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_PercentageDamage, false, 0.f);
	// 0.01 = 1%

	// Calculate Final Damage
	float L_FinalDamage = TargetMaxHealth * Percentage - FMath::Log2(2 + L_DefensePower);
	L_FinalDamage = FMath::Floor(L_FinalDamage) < 1.f ? 1.f : FMath::Floor(L_FinalDamage);

	// Send HitReact Event To Target
	AActor* Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
	FGameplayEventData Data;
	Data.EventMagnitude = L_FinalDamage;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Target,
		SOWGameplayTags::Shared_Event_HitReact,
		Data
	);


	// Apply Final Damage
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedPropertiesPercentageDamage().CurrentHealthDef.AttributeToCapture,
			EGameplayModOp::Additive,
			-L_FinalDamage)
	);
}
