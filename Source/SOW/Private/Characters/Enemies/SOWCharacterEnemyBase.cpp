// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SOWCharacterEnemyBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Structures/Enemies/EnemyStructs.h"


// Sets default values
ASOWCharacterEnemyBase::ASOWCharacterEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASOWCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	FName LEnemyTypeStr = FName(StaticEnum<EEnemyTypes>()->GetDisplayNameTextByValue(static_cast<int64>(EnemyType)).ToString());
	EnemyTypeStr = LEnemyTypeStr;

	if (const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributeData>(EnemyTypeStr, ""))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Success!"));

		GetCharacterMovement()->MaxWalkSpeed = EnemyAttributesData->MaxWalkSpeed;
		
		AttackDamageAmount = EnemyAttributesData->AttackDamageAmount;

		HitAnimation = EnemyAttributesData->HitAnimation;
		DeathAnimation = EnemyAttributesData->DeathAnimation;
		AttackAnimation = EnemyAttributesData->AttackAnimation;
	}
}

void ASOWCharacterEnemyBase::SetAIController(AEnemyBaseAIController* const NewAIController)
{
	AIController = NewAIController;
}

