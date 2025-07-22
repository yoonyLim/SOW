// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/Turrets/GEEC_ApplyTDOnSineCount.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffect.h"
#include "SOWGameplayTags.h"

struct FTrueDamageCaptures {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);

	FTrueDamageCaptures() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, CurrentHealth, Target, false);
	}
};

static const FTrueDamageCaptures& GetTrueDamageData() {
	static FTrueDamageCaptures AttributeCaptures;
	return AttributeCaptures;
}

UGEEC_ApplyTDOnSineCount::UGEEC_ApplyTDOnSineCount()
{
	RelevantAttributesToCapture.Add(GetTrueDamageData().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetTrueDamageData().CurrentHealthDef);
}



void UGEEC_ApplyTDOnSineCount::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	if (!TargetActor) return;

	


	int32 L_SineCount = 0;
	FGameplayTagContainer TagCont;
	CheckDebuffStatus(TargetActor, SOWGameplayTags::Enemy_Status_Debuff_Sin_Alpha, TagCont, L_SineCount);
	CheckDebuffStatus(TargetActor, SOWGameplayTags::Enemy_Status_Debuff_Sin_Beta, TagCont, L_SineCount);
	CheckDebuffStatus(TargetActor, SOWGameplayTags::Enemy_Status_Debuff_Sin_Gamma, TagCont, L_SineCount);
	CheckDebuffStatus(TargetActor, SOWGameplayTags::Enemy_Status_Debuff_Sin_Delta, TagCont, L_SineCount);
	TargetASC->RemoveActiveEffectsWithTags(TagCont);

	UE_LOG(LogTemp, Warning, TEXT("TD Count : %s"), *FString::FromInt(L_SineCount));

	float L_AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetTrueDamageData().AttackPowerBaseDef,
		EvalParams,
		L_AttackPower
	);
	L_AttackPower /= 2.0f;
	L_AttackPower *= L_SineCount;

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetTrueDamageData().CurrentHealthDef.AttributeToCapture,
			EGameplayModOp::Additive,
			-L_AttackPower)
	);
}

void CheckDebuffStatus(AActor* TargetActor, FGameplayTag InTag, FGameplayTagContainer& TagCont, int32& SineCount)
{
	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(TargetActor, InTag)) {
		SineCount++;
		TagCont.AddTag(InTag);
	}
}
