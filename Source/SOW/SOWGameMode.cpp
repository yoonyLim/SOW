// Copyright Epic Games, Inc. All Rights Reserved.

#include "SOWGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

ASOWGameMode::ASOWGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/01Blueprints/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
