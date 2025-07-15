// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_FollowIncomingRoute.h"

#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/Enemies/EnemyIncomingRouteComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_FollowIncomingRoute::UBTT_FollowIncomingRoute(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Follow Incoming Route";
}

EBTNodeResult::Type UBTT_FollowIncomingRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASOWCharacterEnemyBase* const Enemy = Cast<ASOWCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		FVector IndexGoalPos = Enemy->GetEnemyIncomingRouteComponent()->GetCurrentIndexPosition();
		Enemy->GetEnemyIncomingRouteComponent()->IncrementIncomingRouteIndex();

		FAIMoveRequest MoveReq;
		MoveReq.SetGoalLocation(IndexGoalPos);
		MoveReq.SetAcceptanceRadius(AcceptanceRadius);

		FNavPathSharedPtr NavPath;
		FPathFollowingRequestResult MoveRes = OwnerComp.GetAIOwner()->MoveTo(MoveReq, &NavPath);

		if (MoveRes.Code == EPathFollowingRequestResult::RequestSuccessful)
		{
			// OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddUObejct(this, &UBTT_FollowIncomingRoute::OnMoveCompleted);
		}
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_FollowIncomingRoute::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
}
