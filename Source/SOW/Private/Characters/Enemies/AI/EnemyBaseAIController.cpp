// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemyBaseAIController.h"

void AEnemyBaseAIController::SetupPerceptionSystem()
{
}

void AEnemyBaseAIController::OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus)
{
}

void AEnemyBaseAIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}

AEnemyBaseAIController::AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer)
{
}

void AEnemyBaseAIController::InitializeBlackBoard(float StrafeRadius, float AttackRadius, float AttackRate)
{
}
