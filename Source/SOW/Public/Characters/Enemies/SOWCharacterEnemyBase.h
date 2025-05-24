// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWCharacterEnemyBase.generated.h"

class AEnemyBaseAIController;
class UBehaviorTree;

UCLASS()
class SOW_API ASOWCharacterEnemyBase : public ASOWCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASOWCharacterEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AEnemyBaseAIController* MAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* MBehaviorTree;

public:
	// GETTERS
	FORCEINLINE AEnemyBaseAIController* GetAIController() const { return MAIController; };
	UBehaviorTree* GetBehaviorTree() const { return MBehaviorTree; };

	// SETTERS
	void SetAIController(AEnemyBaseAIController* const AIController);
};
