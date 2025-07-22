// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FollowIncomingRoute.generated.h"

struct FPathFollowingResult;
/**
 * 
 */
UCLASS()
class SOW_API UBTT_FollowIncomingRoute : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY()
	AAIController* CachedController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 100.f;

public:
	explicit UBTT_FollowIncomingRoute(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
};
