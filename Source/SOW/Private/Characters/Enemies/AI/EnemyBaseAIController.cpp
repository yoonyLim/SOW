// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemyBaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Characters/SOWCharacter.h"
#include "Characters/CoreRune/SOWCharacterCoreRune.h"
#include "Enumerations/Enemies/EnemyEnums.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyBaseAIController::AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyBaseAIController::SetupPerceptionSystem()
{
	MSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (MSightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Comp")));

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
	ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(SeenTarget);
	if (!SOWCharacter) return;
	
	ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();
	EEnemyTargetPriority TargetPriority = Cast<ASOWCharacterEnemyBase>(GetPawn())->GetTargetPriority();

	if (bIsTaunted) return;
	
	if (TargetType == ESOWCharacterType::CoreRune && TargetPriority == EEnemyTargetPriority::CoreRune)
	{
		GetBlackboardComponent()->SetValueAsObject("AttackTarget", SeenTarget);
		// UpdateCurrentState(EEnemyStates::Attacking);
	}
	else if (TargetType == ESOWCharacterType::Player && TargetPriority == EEnemyTargetPriority::Player)
	{
		GetBlackboardComponent()->SetValueAsObject("AttackTarget", SeenTarget);
	}
	else if (TargetType == ESOWCharacterType::Turret && TargetPriority == EEnemyTargetPriority::Turret)
	{
		GetBlackboardComponent()->SetValueAsObject("AttackTarget", SeenTarget);
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

			Enemy->SetAIController(this);

			if (AActor* CoreRune = UGameplayStatics::GetActorOfClass(GetWorld(), ASOWCharacterCoreRune::StaticClass()))
				GetBlackboardComponent()->SetValueAsObject("AttackTarget", CoreRune);
			
			UpdateCurrentState(EEnemyStates::FollowingIncomingRoute);
			
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void AEnemyBaseAIController::InitializeBlackBoard(float AttackRadius, float AttackSpeed, EEnemyTargetPriority TargetPriority)
{
	GetBlackboardComponent()->SetValueAsFloat("AttackRadius", AttackRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackSpeed", AttackSpeed);
	GetBlackboardComponent()->SetValueAsEnum("TargetPriority", static_cast<uint8>(TargetPriority));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("Example text that prints a float: %f"), AttackRadius));
}

void AEnemyBaseAIController::UpdateCurrentState(EEnemyStates NewState)
{
	MCurrentState = NewState;
	GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(MCurrentState));
}

void AEnemyBaseAIController::Taunt(AActor* TargetActor, float Duration)
{
	CachedAttackTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		
	bIsTaunted = true;
	GetBlackboardComponent()->SetValueAsObject("AttackTarget", TargetActor);
	UpdateCurrentState(EEnemyStates::Taunted);

	GetWorldTimerManager().SetTimer(
		ResetTauntHandle,
		FTimerDelegate::CreateLambda([&]()
		{
			bIsTaunted = false;
			GetBlackboardComponent()->SetValueAsObject("AttackTarget", CachedAttackTarget.Get());
			UpdateCurrentState(EEnemyStates::FollowingIncomingRoute);
		}),
		Duration,
		false
	);
}