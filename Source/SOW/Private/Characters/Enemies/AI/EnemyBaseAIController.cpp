// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemyBaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Enumerations/Enemies/EnemyEnums.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Structures/Enemies/EnemyStructs.h"

AEnemyBaseAIController::AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyBaseAIController::SetupPerceptionSystem()
{
	MSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (MSightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Compo")));

		MSightConfig->SightRadius = 1000.f;
		MSightConfig->LoseSightRadius = MSightConfig->SightRadius + 100.f;
		MSightConfig->PeripheralVisionAngleDegrees = 60.f;
		MSightConfig->SetMaxAge(5.f);
		MSightConfig->AutoSuccessRangeFromLastSeenLocation = 500.f;
		MSightConfig->DetectionByAffiliation.bDetectEnemies = true;
		MSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		MSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*MSightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBaseAIController::OnTargetSighted);
		GetPerceptionComponent()->ConfigureSense(*MSightConfig);
	}
}

void AEnemyBaseAIController::OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus)
{
	if (ASOWCharacterPlayer* const Player = Cast<ASOWCharacterPlayer>(SeenTarget))
	{
		GetBlackboardComponent()->SetValueAsObject("AttackTarget", Player);
		UpdateCurrentState(EEnemyStates::Attacking);
	}
}

void AEnemyBaseAIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (ASOWCharacterEnemyBase* const Enemy = Cast<ASOWCharacterEnemyBase>(PossessedPawn))
	{
		if (UBehaviorTree* const BehaviorTree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* Bboard;
			UseBlackboard(BehaviorTree->BlackboardAsset, Bboard);
			Blackboard = Bboard; // "Blackboard" is an already existing variable name in AAIController class

			UpdateCurrentState(EEnemyStates::Passive);
			
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void AEnemyBaseAIController::InitializeBlackBoard(float AttackRadius, float AttackRate)
{
	GetBlackboardComponent()->SetValueAsFloat("AttackRadius", AttackRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackRate", AttackRate);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("Example text that prints a float: %f"), AttackRadius));
}

void AEnemyBaseAIController::UpdateCurrentState(EEnemyStates NewState)
{
	MCurrentState = NewState;
	GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(MCurrentState));
}
