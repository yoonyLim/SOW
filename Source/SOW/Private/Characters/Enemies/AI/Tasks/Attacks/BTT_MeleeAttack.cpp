// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"

UBTT_MeleeAttack::UBTT_MeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Melee Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASOWCharacterEnemyBase* const Enemy = Cast<ASOWCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		if (!SOWCharacter) return EBTNodeResult::Failed;
	
		ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();
	
		if (TargetType == ESOWCharacterType::CoreRune || TargetType == ESOWCharacterType::Player || TargetType == ESOWCharacterType::Turret)
		{
			OnAttackMontageEnded.BindUObject(this, &UBTT_MeleeAttack::OnAttackEnded);

			if (ASOWCharacter* const TargetActor = Cast<ASOWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
			{
				bIsAttacking = true;
				Enemy->Attack(TargetActor);
			}

			Enemy->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnAttackMontageEnded); // montage interrupted
			Enemy->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnAttackMontageEnded); // montage ended

			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsAttacking)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_MeleeAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsAttacking = false;
}