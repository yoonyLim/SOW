// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_FollowIncomingRoute.h"

#include "BehaviorTree/BlackboardComponent.h"
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
		FVector TargetLocation = Enemy->GetEnemyIncomingRouteComponent()->GetCurrentIndexPosition();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", TargetLocation);

		if (Enemy->GetEnemyIncomingRouteComponent()->IncrementIncomingRouteIndex())
		{
			Enemy->GetAIController()->UpdateCurrentState(EEnemyStates::Attacking);
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;	
		
		/* CachedOwnerComp = &OwnerComp;
		CachedController = OwnerComp.GetAIOwner();
		
		AEnemyIncomingRoute* IncomingRoute = Enemy->GetEnemyIncomingRouteComponent()->GetIncomingRoute();

		FVector TargetLocation = Enemy->GetEnemyIncomingRouteComponent()->GetCurrentIndexPosition();
		UE_LOG(LogTemp, Warning, TEXT("UBTT_FollowIncomingRoute: Moving to TargetLocation: %s"), *TargetLocation.ToString());

		FAIMoveRequest MoveReq;
		MoveReq.SetGoalLocation(TargetLocation);
		MoveReq.SetAcceptanceRadius(AcceptanceRadius);

		CachedController->ReceiveMoveCompleted.AddUniqueDynamic(this, &UBTT_FollowIncomingRoute::OnMoveCompleted);

		FNavPathSharedPtr NavPath;
		FPathFollowingRequestResult MoveRes = OwnerComp.GetAIOwner()->MoveTo(MoveReq, &NavPath);

		if (MoveRes.Code == EPathFollowingRequestResult::RequestSuccessful)
		{
			return EBTNodeResult::InProgress; // Task is now running and waiting for move completion
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UBTT_FollowIncomingRoute: AI MoveTo failed with code %d. Task failed."), (int32)MoveRes.Code);
			// Clean up the delegate registration if the move failed immediately
			CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_FollowIncomingRoute::OnMoveCompleted);
			return EBTNodeResult::Failed;
		}

		if (MoveRes.Code == EPathFollowingRequestResult::RequestSuccessful)
		{
			// OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddUObejct(this, &UBTT_FollowIncomingRoute::OnMoveCompleted);
		}*/
	}
	
	return EBTNodeResult::Failed;
}

/*void UBTT_FollowIncomingRoute::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	// Ensure we have the owner component
	UBehaviorTreeComponent* OwnerComp = CachedOwnerComp;
	if (!OwnerComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_FollowIncomingRoute::OnMoveCompleted: OwnerComp is null."));
		// If OwnerComp is null, we can't finish the task, just return.
		return;
	}

	AController* Controller = OwnerComp->GetAIOwner();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_FollowIncomingRoute::OnMoveCompleted: No AI Controller found for completed move."));
		// We still need to call FinishLatentTask to avoid the BT being stuck in InProgress
		// Even if the controller is gone, the BT might still be active.
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed); 
		return;
	}

	// Remove the delegate once the move is completed
	// It's safer to remove specific dynamic delegates by matching the FName.
	// If the name is missing, it will remove all delegates bound to this UObject.
	CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_FollowIncomingRoute::OnMoveCompleted);


	ASOWCharacterEnemyBase* Enemy = Cast<ASOWCharacterEnemyBase>(Controller->GetCharacter());
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_FollowIncomingRoute::OnMoveCompleted: Controlled pawn is not ASOWCharacterEnemyBase. Task failed."));
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Result == EPathFollowingResult::Success) // Fixed comparison
	{
		// Increment the spline point index for the next move
		Enemy->GetEnemyIncomingRouteComponent()->IncrementIncomingRouteIndex();

		// Re-execute this task to move to the next point
		// CachedOwnerComp->RequestExecution(this, EBTNodeResult::InProgress); 
	}
	else // Move failed (e.g., could not path, aborted)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_FollowIncomingRoute: Move to point failed or was aborted. Reason: %s"), *UEnum::GetValueAsString(Result)); // Fixed logging
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed); // Inform BT that task failed
	}
}*/
