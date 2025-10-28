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
	//const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	//
	//FAggregatorEvaluateParameters EvalParams;
	//EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	//EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	//

	//AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	//if (!SourceActor) {
	//	UE_LOG(LogTemp, Error, TEXT("The Actor in not Turret or The Actor don't have valid ASC."));
	//	return;
	//}
	//
	//
	//ASOWCharacterTurretBase* SourceTurret = Cast<ASOWCharacterTurretBase>(SourceActor);
	//if (!SourceTurret) return;

	//USOWTurretCombatComponent* SourceTurretComp = SourceTurret->FindComponentByClass<USOWTurretCombatComponent>();
	//if (!SourceTurretComp) return;
	//
	////int32 TargetCircleCount = SourceTurret->GetCircleCount();

	//FWidgetDesciptableTurretAttribute Data;




	///* For AttackPower Refresh */

	//// 현상태, 단순히 지금 적용되어있는 스텟의 값 자체를 베이스로 구조체의 데이터로 처리하는 중임.

	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
	//	GetCapturedCircleData().AttackPowerBaseDef,
	//	EvalParams,
	//	Data.AttackPowerBaseValue
	//);



	///* For AttackSpeed Refresh */
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
	//	GetCapturedCircleData().AttackSpeedBaseDef,
	//	EvalParams,
	//	Data.AttackSpeedBaseValue
	//);

	//SourceTurretComp->SetWidgetDecriptableAttributes(Data);
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	if (!SourceActor) {
		UE_LOG(LogTemp, Error, TEXT("Invalid SourceActor or missing ASC."));
		return;
	}

	ASOWCharacterTurretBase* SourceTurret = Cast<ASOWCharacterTurretBase>(SourceActor);
	if (!SourceTurret) return;

	USOWTurretCombatComponent* SourceTurretComp = SourceTurret->FindComponentByClass<USOWTurretCombatComponent>();
	if (!SourceTurretComp) return;

	FWidgetDesciptableTurretAttribute Data;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	if (!SourceASC) return;

	// 여기서 "기본값(BaseValue)"만 가져오기
	const FGameplayAttribute AttackPowerAttr = GetCapturedCircleData().AttackPowerBaseDef.AttributeToCapture;
	const FGameplayAttribute AttackSpeedAttr = GetCapturedCircleData().AttackSpeedBaseDef.AttributeToCapture;

	Data.AttackPowerBaseValue = SourceASC->GetNumericAttributeBase(AttackPowerAttr);
	Data.AttackSpeedBaseValue = SourceASC->GetNumericAttributeBase(AttackSpeedAttr);

	// 필요 시 현재값(버프 포함)을 따로 저장하고 싶다면 다음도 가능:
	// float CurrentAttackPower = SourceASC->GetNumericAttribute(AttackPowerAttr);
	// float CurrentAttackSpeed = SourceASC->GetNumericAttribute(AttackSpeedAttr);

	SourceTurretComp->SetWidgetDecriptableAttributes(Data);
	
}