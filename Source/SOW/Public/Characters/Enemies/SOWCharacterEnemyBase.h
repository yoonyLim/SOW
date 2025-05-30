// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "Enumerations/Enemies/EnemyEnums.h"
#include "Interface/EnemyActionsInterface.h"
#include "SOWCharacterEnemyBase.generated.h"

class AEnemyBaseAIController;
class UBehaviorTree;

UCLASS()
class SOW_API ASOWCharacterEnemyBase : public ASOWCharacter, public IEnemyActionsInterface
{
	GENERATED_BODY()

	FName EnemyTypeStr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	EEnemyTypes EnemyType;

public:
	// Sets default values for this character's properties
	ASOWCharacterEnemyBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "EnemyAttributeData"))
	FDataTableRowHandle EnemyAttributesDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AEnemyBaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

public:
	// GETTERS
	FORCEINLINE AEnemyBaseAIController* GetAIController() const { return AIController; };
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; };
	EEnemyTypes GetEnemyType() const { return EnemyType; };
	FName GetEnemyTypeStr() const { return EnemyTypeStr; };

	virtual void Attack(const ASOWCharacter* TargetActor) override;
};
