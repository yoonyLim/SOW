// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/CoreRune/GEEC_InitCoreRuneAttributes.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Characters/CoreRune/SOWCharacterCoreRune.h"

struct FAttributeCaptures
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthBase);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);

	FAttributeCaptures()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MaxHealthBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, CurrentHealth, Source, false);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Source, false);
	}
};

static const FAttributeCaptures& GetCapturedCoreRuneAttributes()
{
	static FAttributeCaptures CoreRuneAttributeCaptures;
	return CoreRuneAttributeCaptures;
}

UGEEC_InitCoreRuneAttributes::UGEEC_InitCoreRuneAttributes()
{
	RelevantAttributesToCapture.Add(GetCapturedCoreRuneAttributes().MaxHealthBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedCoreRuneAttributes().CurrentHealthDef);
}

void UGEEC_InitCoreRuneAttributes::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	if (ASOWCharacterCoreRune* CoreRune = Cast<ASOWCharacterCoreRune>(ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor()))
	{
		// const auto EnemyAttributesData = Enemy->EnemyAttributesDT.DataTable->FindRow<FEnemyAttributeData>(Enemy->GetEnemyTypeStr(), "");

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCapturedCoreRuneAttributes().MaxHealthBaseDef.AttributeToCapture,
				EGameplayModOp::Override,
				100.f
				//EnemyAttributesData->MaxHealth
			)
		);

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetCapturedCoreRuneAttributes().CurrentHealthDef.AttributeToCapture,
				EGameplayModOp::Override,
				100.f
				//EnemyAttributesData->MaxHealth
			)
		);
	}
}
