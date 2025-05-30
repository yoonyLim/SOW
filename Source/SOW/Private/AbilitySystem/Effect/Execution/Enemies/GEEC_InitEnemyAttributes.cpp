// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/Enemies/GEEC_InitEnemyAttributes.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffect.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Structures/Enemies/EnemyStructs.h"

struct FAttributeCaptures
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPowerBase);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);

	FAttributeCaptures()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeed, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Source, false);
	}
};

static const FAttributeCaptures& GetCapturedEnemyAttributes()
{
	static FAttributeCaptures EnemyAttributeCaptures;
	return EnemyAttributeCaptures;
}

// AActor* actor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor() // execute function call
UGEEC_InitEnemyAttributes::UGEEC_InitEnemyAttributes()
{
	RelevantAttributesToCapture.Add(GetCapturedEnemyAttributes().MaxHealthBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedEnemyAttributes().AttackSpeedDef);
	RelevantAttributesToCapture.Add(GetCapturedEnemyAttributes().AttackPowerBaseDef);
	// RelevantAttributesToCapture.Add(GetCapturedEnemyAttributes().DefensePowerBaseDef);
}

void UGEEC_InitEnemyAttributes::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	if (ASOWCharacterEnemyBase* Enemy = Cast<ASOWCharacterEnemyBase>(ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor()))
	{
		const auto EnemyAttributesData = Enemy->EnemyAttributesDT.DataTable->FindRow<FEnemyAttributeData>(Enemy->GetEnemyTypeStr(), "");

		/*if (EnemyAttributesData)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Enemy->GetEnemyTypeStr().ToString());
		}*/

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCapturedEnemyAttributes().MaxHealthBaseDef.AttributeToCapture,
				EGameplayModOp::Override,
				EnemyAttributesData->MaxHealth
			)
		);

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCapturedEnemyAttributes().AttackSpeedDef.AttributeToCapture,
				EGameplayModOp::Override,
				EnemyAttributesData->AttackSpeed
			)
		);

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCapturedEnemyAttributes().AttackPowerBaseDef.AttributeToCapture,
				EGameplayModOp::Override,
				EnemyAttributesData->AttackPowerBase
			)
		);

		/*OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCapturedEnemyAttributes().DefensePowerBaseDef.AttributeToCapture,
				EGameplayModOp::Override,
				EnemyAttributesData->DefensePowerBase
			)
		);*/
	}
}
