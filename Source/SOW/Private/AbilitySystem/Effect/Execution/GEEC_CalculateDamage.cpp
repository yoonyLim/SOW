// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_CalculateDamage.h"
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

struct FAttributeCapturesDamage {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraDamageRatio);


	FAttributeCapturesDamage() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, CurrentHealth, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, ExtraDamageRatio, Target, false);
	}
};

static const FAttributeCapturesDamage& GetCapturedPropertiesDamage() {
	static FAttributeCapturesDamage AttributeCaptures;
	return AttributeCaptures;
}

float UGEEC_CalculateDamage::GetElementalResistanceCost(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvalParams) const
{
	FGameplayTag RootTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element"));
	TArray<FGameplayTag> MatchedElementTags;

	if (EvalParams.SourceTags)
	{
		for (const FGameplayTag& Tag : *EvalParams.SourceTags)
		{
			if (Tag.MatchesTag(RootTag)) // "Shared.Element" 하위 태그 포함 필터
			{
				MatchedElementTags.Add(Tag);
			}
		}
	}
	/* 타겟의 속성 저항치를 불러오기 위한 코드 */
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	float ElementalResistance = 0.f;
	const USOWAttributeSet* TargetAttributes = Cast<USOWAttributeSet>(TargetASC->GetAttributeSet(USOWAttributeSet::StaticClass()));
	if (TargetAttributes)
	{
		for (const FGameplayTag& ElementTag : MatchedElementTags) {
			ElementalResistance += TargetAttributes->GetResistanceForElementWithElementTag(ElementTag);
		}

	}
	UE_LOG(LogTemp, Warning, TEXT("ElementalResistance : %f"), ElementalResistance);

	return ElementalResistance;
}

UGEEC_CalculateDamage::UGEEC_CalculateDamage()
{
	RelevantAttributesToCapture.Add(GetCapturedPropertiesDamage().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesDamage().DefensePowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesDamage().CurrentHealthDef);

	RelevantAttributesToCapture.Add(GetCapturedPropertiesDamage().ExtraDamageRatioDef);
}

void UGEEC_CalculateDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] Execute_Implementation called.")); //로그 추가, 나중에 지울것.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();



	// Base Attack Power
	float L_AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesDamage().AttackPowerBaseDef,
		EvalParams,
		L_AttackPower
	);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] AttackPower captured: Value: %f"), L_AttackPower); // 로그추가
	L_AttackPower += Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_AdditiveDamage, false, 0.0f);
	L_AttackPower *= Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_MultipleDamage, false, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] AttackPower Final : Value: %f"), L_AttackPower); // 로그추가

	float L_ExtraRatio = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesDamage().ExtraDamageRatioDef,
		EvalParams,
		L_ExtraRatio
	);
	
	// Elemantal Damage
	float ElementalResistance = 1.f;
	//float ElementalResistance = GetElementalResistanceCost(ExecutionParams, EvalParams);
	//ElementalResistance = ElementalResistance > 0.2f ? ElementalResistance : 0.2f;
	//UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] ElementalResistance: %f"), ElementalResistance); //로그추가


	// Additinal Damage By Sine Debuff
	float AdditinalDamageRatio = 0.f;
	AdditinalDamageRatio += Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_AdditinalDamageRatio_Pulse, false, 0.f);

	//UE_LOG(LogTemp, Warning, TEXT("Additional Damage Ratio : %s"), *FString::SanitizeFloat(AdditinalDamageRatio, 2));
	//UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] AdditionalDamageRatio: %f"), AdditinalDamageRatio); //로그추가

	// Base Defense Power
	float L_DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesDamage().DefensePowerBaseDef,
		EvalParams,
		L_DefensePower
	);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] DefensePower captured Value: %f"), L_DefensePower); //로그추가

	float BasicDamageFormal = (L_AttackPower - FMath::Log2(2 + L_DefensePower));
	float ElementalExtraDamage = ElementalResistance;
	float FinalExtraDamage = (1.0f + L_ExtraRatio + AdditinalDamageRatio);

	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] FinalExtraDamage : Value: %f"), FinalExtraDamage); // 로그추가
	// Calculate Final Damage
	float L_FinalDamage = BasicDamageFormal * ElementalExtraDamage * FinalExtraDamage;
	L_FinalDamage = FMath::CeilToInt(L_FinalDamage) < 1.f ? 1.f : FMath::CeilToInt(L_FinalDamage);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] FinalDamage calculated: %f"), L_FinalDamage); // 로그추가

	// Send HitReact Event To Target
	AActor* Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
	FGameplayEventData Data;
	Data.Instigator = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	Data.EventMagnitude = L_FinalDamage;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Target,
		SOWGameplayTags::Shared_Event_HitReact,
		Data
	);


	// Apply Final Damage
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedPropertiesDamage().CurrentHealthDef.AttributeToCapture,
			EGameplayModOp::Additive,
			-L_FinalDamage)
	);
}
