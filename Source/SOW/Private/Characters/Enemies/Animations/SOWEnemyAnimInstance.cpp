// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Animations/SOWEnemyAnimInstance.h"

#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void USOWEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<ASOWCharacterEnemyBase>(TryGetPawnOwner());
}

void USOWEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if (Enemy)
	{
		Velocity = Enemy->GetCharacterMovement()->Velocity;

		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Enemy->GetActorRotation());
	}
	
}
