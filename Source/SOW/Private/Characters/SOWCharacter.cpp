// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/SOWCharacter.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"

#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASOWCharacter

ASOWCharacter::ASOWCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<USOWAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<USOWAttributeSet>(TEXT("AttributeSet"));
}
