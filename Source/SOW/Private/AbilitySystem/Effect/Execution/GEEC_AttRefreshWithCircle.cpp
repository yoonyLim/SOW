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
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeedBase);



	FRefreshAttributeCaptures() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackPowerBase, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, AttackSpeedBase, Source, false);
	}
};

static const FRefreshAttributeCaptures& GetCapturedCircleData() {
	static FRefreshAttributeCaptures AttributeCaptures;
	return AttributeCaptures;
}

UGEEC_AttRefreshWithCircle::UGEEC_AttRefreshWithCircle()
{
	RelevantAttributesToCapture.Add(GetCapturedCircleData().AttackPowerBaseDef);
	RelevantAttributesToCapture.Add(GetCapturedCircleData().AttackSpeedBaseDef);

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




	/* For AttackPower Refresh */

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().AttackPowerBaseDef,
		EvalParams,
		Data.AttackPowerBaseValue
	);



	/* For AttackSpeed Refresh */
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetCapturedCircleData().AttackSpeedBaseDef,
		EvalParams,
		Data.AttackSpeedBaseValue
	);

	SourceTurretComp->SetWidgetDecriptableAttributes(Data);
	
}