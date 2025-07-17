// Fill out your copyright notice in the Description page of Project Settings.



#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_RangedAttack.generated.h"

UCLASS()
class SOW_API UBTT_RangedAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	// 애니메이션 몽타주가 끝났을 때 호출될 델리게이트
	FOnMontageEnded OnAttackMontageEnded;

	// 공격이 끝났는지 플래그
	bool bIsAttacking = false;

	// 몽타주 종료 콜백
	void OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted);

public:
	UBTT_RangedAttack(FObjectInitializer const& ObjectInitializer);

	// Task 시작


	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;

	// 매 Tick 마다 상태 체크
	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;
};