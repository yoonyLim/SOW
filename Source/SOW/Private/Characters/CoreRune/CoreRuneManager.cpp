// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CoreRune/CoreRuneManager.h"
#include "Characters/CoreRune/SOWCharacterCoreRune.h"
#include "Kismet/GameplayStatics.h"

void UCoreRuneManager::MakeCoreRuneReference(ASOWCharacterCoreRune* InRune)
{
	if (CoreRune) return;

	CoreRune = InRune;
}

void UCoreRuneManager::Initialize() {
	//MakeCoreRuneReference();

	//UE_LOG(LogTemp, Warning, TEXT("CoreRune Found : %s"), *CoreRune->GetActorNameOrLabel());;
}



ASOWCharacterCoreRune* UCoreRuneManager::GetCoreRune() const
{
	return CoreRune;
}
