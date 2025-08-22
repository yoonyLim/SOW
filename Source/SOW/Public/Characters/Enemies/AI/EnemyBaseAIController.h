// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enumerations/Enemies/EnemyEnums.h"

#include "EnemyBaseAIController.generated.h"

UCLASS()
class SOW_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY()
	class UAISenseConfig_Sight* MSightConfig;

	// void SetupPerceptionSystem();

	/*UFUNCTION()
	void OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus);*/

	EEnemyStates MCurrentState;

	FTimerHandle ResetTauntHandle;
	bool bIsTaunted = false;

	UPROPERTY()
	TWeakObjectPtr<AActor> CachedAttackTarget = nullptr;

protected:
	virtual void OnPossess(APawn* PossessedPawn) override;
	
public:
	// AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer);

	void InitializeBlackBoard(float AttackRadius, float AttackRate, EEnemyTargetPriority TargetPriority);

	FORCEINLINE EEnemyStates GetCurrentState() const { return MCurrentState; }

	void UpdateCurrentState(EEnemyStates NewState);

	UFUNCTION(BlueprintCallable)
	void Taunt(AActor* TargetActor, float Duration);
};
