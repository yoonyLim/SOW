// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SOWCharacterEnemyBase.h"

#include "SOWGameplayTags.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structures/Enemies/EnemyStructs.h"
#include "Characters/Enemies/SOWEnemyCombatComponent.h"


// Sets default values
ASOWCharacterEnemyBase::ASOWCharacterEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterType = ESOWCharacterType::Enemy;


	// EnemyCombatComponent ¿¬°á
	EnemyCombatComponent = CreateDefaultSubobject<USOWEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

// Called when the game starts or when spawned
void ASOWCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	FName LEnemyTypeStr = FName(StaticEnum<EEnemyTypes>()->GetDisplayNameTextByValue(static_cast<int64>(EnemyType)).ToString());
	EnemyTypeStr = LEnemyTypeStr;

	if (const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributeData>(EnemyTypeStr, ""))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, EnemyTypeStr.ToString());

		GetCharacterMovement()->MaxWalkSpeed = EnemyAttributesData->MaxWalkSpeed;

		HitAnimation = EnemyAttributesData->HitAnimation;
		DeathAnimation = EnemyAttributesData->DeathAnimation;
		AttackAnimation = EnemyAttributesData->AttackAnimation;

		AIController = Cast<AEnemyBaseAIController>(GetController());
		
		if (AIController)
			AIController->InitializeBlackBoard(EnemyAttributesData->AttackRadius, EnemyAttributesData->AttackSpeed);
	}

	// To initialize Game Ability Attribute
	AbilitySystemComponent->AddLooseGameplayTag(SOWGameplayTags::Enemy_Ability_Initialize);
}

void ASOWCharacterEnemyBase::Attack(const ASOWCharacter* TargetActor)
{
	if (AttackAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(AttackAnimation);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack Animation Played!"));
	}
}

