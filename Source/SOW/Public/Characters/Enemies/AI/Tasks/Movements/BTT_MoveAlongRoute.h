

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveAlongRoute.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UBTT_MoveAlongRoute : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_MoveAlongRoute();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
