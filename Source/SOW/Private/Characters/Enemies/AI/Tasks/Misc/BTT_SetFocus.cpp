// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Misc/BTT_SetFocus.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/SOWCharacter.h"

UBTT_SetFocus::UBTT_SetFocus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Focus";
}

EBTNodeResult::Type UBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		if (!SOWCharacter) return EBTNodeResult::Failed;
	
		ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();
	
		if (TargetType == ESOWCharacterType::Player || TargetType == ESOWCharacterType::Turret)
		{
			if (ASOWCharacter* TargetActor = Cast<ASOWCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
				EnemyController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}