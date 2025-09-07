// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_SyncSpeedWithAttributeEnemy.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffect.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

struct FEnemyWalkSpeedCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(WalkSpeed);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePowerBase);

	FEnemyWalkSpeedCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, WalkSpeed, Source, false);
		
		// DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DefensePowerBase, Source, false);
	}
};

static const FEnemyWalkSpeedCapture& GetCapturedEnemyWalkSpeed()
{
	static FEnemyWalkSpeedCapture EnemyAttributeCapture;
	return EnemyAttributeCapture;
}



UGEEC_SyncSpeedWithAttributeEnemy::UGEEC_SyncSpeedWithAttributeEnemy()
{
	RelevantAttributesToCapture.Add(GetCapturedEnemyWalkSpeed().WalkSpeedDef);
}

void UGEEC_SyncSpeedWithAttributeEnemy::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	if (ASOWCharacterEnemyBase* Enemy = Cast<ASOWCharacterEnemyBase>(ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor()))
	{
		UAbilitySystemComponent* ASC = ExecutionParams.GetSourceAbilitySystemComponent();
		if (ASC)
		{
			ASC->SetNumericAttributeBase(
				GetCapturedEnemyWalkSpeed().WalkSpeedDef.AttributeToCapture,
				Enemy->GetCharacterMovement()->MaxWalkSpeed
			);
		}
	}
}
