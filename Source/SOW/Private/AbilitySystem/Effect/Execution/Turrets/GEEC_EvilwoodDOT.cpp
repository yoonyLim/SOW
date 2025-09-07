// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/Turrets/GEEC_EvilwoodDOT.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffect.h"
#include "SOWGameplayTags.h"
// TO-DO
/*

	SOWAttributeSet으로부터 캡처해야 할 각 스텟을 정의함.
	캡처할 스텟들의 출처 (공격 : 이펙트 제공자 / 방어 : 이펙트 대상자 etc)를 정의함.
	기획안에 따른 데미지 로직을 구현함.

*/

struct FAttributeCapturesDoT {
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageOverTime);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);


	FAttributeCapturesDoT() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DamageOverTime, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, CurrentHealth, Source, false);
	}
};

static const FAttributeCapturesDoT& GetCapturedPropertiesDoT() {
	static FAttributeCapturesDoT AttributeCaptures;
	return AttributeCaptures;
}



UGEEC_EvilwoodDOT::UGEEC_EvilwoodDOT()
{
	RelevantAttributesToCapture.Add(GetCapturedPropertiesDoT().DamageOverTimeDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesDoT().DefensePowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesDoT().CurrentHealthDef);
}

void UGEEC_EvilwoodDOT::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] Execute_Implementation called.")); //로그 추가, 나중에 지울것.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();



	// Base Attack Power
	float L_Dot = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesDoT().DamageOverTimeDef,
		EvalParams,
		L_Dot
	);

	// Base Defense Power
	float L_DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesDoT().DefensePowerBaseDef,
		EvalParams,
		L_DefensePower
	);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] DefensePower captured Value: %f"), L_DefensePower); //로그추가


	// Calculate Final Damage
	float L_FinalDamage = (L_Dot - FMath::Log2(2 + L_DefensePower));
	L_FinalDamage = FMath::Floor(L_FinalDamage);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] FinalDamage calculated: %f"), L_FinalDamage); // 로그추가

	// Send HitReact Event To Target
	/*AActor* Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Target,
		SOWGameplayTags::Shared_Event_HitReact,
		FGameplayEventData()
	);*/


	// Apply Final Damage
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedPropertiesDoT().CurrentHealthDef.AttributeToCapture,
			EGameplayModOp::Additive,
			-L_FinalDamage)
	);
}
