// Copyright Epic Games, Inc. All Rights Reserved.

#include "SOWGameMode.h"
#include "SOWCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASOWGameMode::ASOWGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
