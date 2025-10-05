// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_CalculateRangedDamage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "SOWGameplayTags.h"
#include "SOWLogFunctionLibrary.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Characters/SOWCharacter.h"
#include "Widget/DamageLogger.h"

struct FAttributeCapturesRangedDamage {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AdditionalDamageRatio);

	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraDamageRatio);


	FAttributeCapturesRangedDamage() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AdditionalDamageRatio, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, CurrentHealth, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, ExtraDamageRatio, Target, false);
	}
};

static const FAttributeCapturesRangedDamage& GetCapturedPropertiesRangedDamage() {
	static FAttributeCapturesRangedDamage AttributeCaptures;
	return AttributeCaptures;
}

float UGEEC_CalculateRangedDamage::GetElementalResistanceCost(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvalParams) const
{
	FGameplayTag RootTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element"));
	TArray<FGameplayTag> MatchedElementTags;

	if (EvalParams.SourceTags)
	{
		for (const FGameplayTag& Tag : *EvalParams.SourceTags)
		{
			if (Tag.MatchesTag(RootTag)) // "Shared.Element" ���� �±� ���� ����
			{
				MatchedElementTags.Add(Tag);
			}
		}
	}
	/* Ÿ���� �Ӽ� ����ġ�� �ҷ����� ���� �ڵ� */
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

UGEEC_CalculateRangedDamage::UGEEC_CalculateRangedDamage()
{
	RelevantAttributesToCapture.Add(GetCapturedPropertiesRangedDamage().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesRangedDamage().AdditionalDamageRatioDef);

	RelevantAttributesToCapture.Add(GetCapturedPropertiesRangedDamage().DefensePowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedPropertiesRangedDamage().CurrentHealthDef);

	RelevantAttributesToCapture.Add(GetCapturedPropertiesRangedDamage().ExtraDamageRatioDef);
}

void UGEEC_CalculateRangedDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Check if the target has RangedResistance tag
	if (USOWBlueprintFunctionLibrary::DoesActorHasTag(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor(), SOWGameplayTags::Enemy_Status_Buff_RangedResistance))
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] Execute_Implementation called.")); //�α� �߰�, ���߿� �����.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();



	// Base Attack Power
	float L_AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesRangedDamage().AttackPowerBaseDef,
		EvalParams,
		L_AttackPower
	);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] AttackPower captured: Value: %f"), L_AttackPower); // �α��߰�
	L_AttackPower += Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_AdditiveDamage, false, 0.0f);
	L_AttackPower *= Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_MultipleDamage, false, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] AttackPower Final : Value: %f"), L_AttackPower); // �α��߰�

	float L_ExtraRatio = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesRangedDamage().ExtraDamageRatioDef,
		EvalParams,
		L_ExtraRatio
	);
	
	float L_AddtionalRatio = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesRangedDamage().AdditionalDamageRatioDef,
		EvalParams,
		L_AddtionalRatio
	);
	// Elemantal Damage
	float ElementalResistance = 1.f;
	//float ElementalResistance = GetElementalResistanceCost(ExecutionParams, EvalParams);
	//ElementalResistance = ElementalResistance > 0.2f ? ElementalResistance : 0.2f;
	//UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] ElementalResistance: %f"), ElementalResistance); //�α��߰�


	// Additinal Damage By Sine Debuff
	float AdditinalDamageRatio = 0.f;
	AdditinalDamageRatio += Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_AdditinalDamageRatio_Pulse, false, 0.f); 
	AdditinalDamageRatio += Spec.GetSetByCallerMagnitude(SOWGameplayTags::Shared_SetByCaller_AdditiveDamage, false, 0.f);
	//UE_LOG(LogTemp, Warning, TEXT("Additional Damage Ratio : %s"), *FString::SanitizeFloat(AdditinalDamageRatio, 2));
	//UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] AdditionalDamageRatio: %f"), AdditinalDamageRatio); //�α��߰�

	// Base Defense Power
	float L_DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedPropertiesRangedDamage().DefensePowerBaseDef,
		EvalParams,
		L_DefensePower
	);

	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] DefensePower captured Value: %f"), L_DefensePower); //�α��߰�

	float BasicDamageFormal = (L_AttackPower - FMath::Log2(2 + L_DefensePower));
	float ElementalExtraDamage = ElementalResistance;
	float FinalExtraDamage = (1.0f + L_ExtraRatio + AdditinalDamageRatio + L_AddtionalRatio);

	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] FinalExtraDamage : Value: %f"), FinalExtraDamage); // �α��߰�
	// Calculate Final Damage
	float L_FinalDamage = BasicDamageFormal * ElementalExtraDamage * FinalExtraDamage;
	L_FinalDamage = FMath::CeilToInt(L_FinalDamage) < 1.f ? 1.f : FMath::CeilToInt(L_FinalDamage);
	UE_LOG(LogTemp, Warning, TEXT("[DamageCalc] FinalDamage calculated: %f"), L_FinalDamage); // �α��߰�

	// Send HitReact Event To Target
	AActor* Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
	FGameplayEventData Data;
	Data.Instigator = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	UDamageLogger* Logger = NewObject<UDamageLogger>();
	Logger->SetLoggerValue(FMath::CeilToInt(BasicDamageFormal), FMath::CeilToInt(L_FinalDamage));
	Data.OptionalObject = Logger;
	Data.EventMagnitude = L_FinalDamage;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Target,
		SOWGameplayTags::Shared_Event_HitReact,
		Data
	);


	// Apply Final Damage
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedPropertiesRangedDamage().CurrentHealthDef.AttributeToCapture,
			EGameplayModOp::Additive,
			-L_FinalDamage)
	);

	//////////

	AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
	FVector TargetPos = TargetActor ? TargetActor->GetActorLocation() : FVector::ZeroVector;

	// ��: �ͷ� ���� ID (�ͷ� Actor���� �������� ��Ŀ� �°� ���� �ʿ�)
	FString TurretInstanceID = FString::FromInt(reinterpret_cast<int64>(ExecutionParams.GetSourceAbilitySystemComponent()));
	FString TurretID = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor()->GetName();
	FString TargetID = TargetActor ? TargetActor->GetName() : TEXT("Unknown");

	// �ܿ� HP (AttributeSet���� ��������)
	float TargetHP = 0.f;
	if (const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent())
	{
		if (const USOWAttributeSet* TargetAttributes = Cast<USOWAttributeSet>(TargetASC->GetAttributeSet(USOWAttributeSet::StaticClass())))
		{
			TargetHP = TargetAttributes->GetCurrentHealth();
		}
	}

	// CSV ���
	USOWLogFunctionLibrary::LogDamageToCSV(
		TurretInstanceID,
		TurretID,
		TargetID,
		FMath::CeilToInt(BasicDamageFormal),
		FMath::CeilToInt(L_FinalDamage),
		TargetHP,
		TargetPos,
		0, // ���̺�/���� ��ȣ
		ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor(),
		ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor()
	);
}
