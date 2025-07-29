// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CoreRune/SOWCharacterCoreRune.h"


// Sets default values
ASOWCharacterCoreRune::ASOWCharacterCoreRune()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterType = ESOWCharacterType::CoreRune;
}

// Called when the game starts or when spawned
void ASOWCharacterCoreRune::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASOWCharacterCoreRune::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASOWCharacterCoreRune::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

