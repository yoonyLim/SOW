


#include "Characters/Enemies/AI/Tasks/Movements/BTT_MoveAlongRoute.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/Enemies/EnemyIncomingRouteComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveAlongRoute::UBTT_MoveAlongRoute()
{
	NodeName = "Move Along Route";
}

EBTNodeResult::Type UBTT_MoveAlongRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASOWCharacterEnemyBase* const Enemy = Cast<ASOWCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (Enemy->GetEnemyIncomingRouteComponent()->HasReachedEnd())
		{
			Enemy->GetAIController()->UpdateCurrentState(EEnemyStates::Attacking);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}

		FVector TargetLocation = Enemy->GetEnemyIncomingRouteComponent()->GetCurrentIndexPosition();
		// OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", TargetLocation);
		
		if (FVector::Dist(TargetLocation, Enemy->GetActorLocation()) <= 5.f) {
			Enemy->GetEnemyIncomingRouteComponent()->IncrementIncomingRouteIndex();
		}

		FVector DirectionVec = (TargetLocation - Enemy->GetActorLocation()).GetSafeNormal();
		FRotator DirectionRot = DirectionVec.Rotation();

		Enemy->SetActorRotation(FRotator(0.f, DirectionRot.Yaw, 0.f));
		Enemy->AddMovementInput(Enemy->GetActorForwardVector(), 1.f);
	}

	return EBTNodeResult::InProgress;
}

