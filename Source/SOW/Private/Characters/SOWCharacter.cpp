// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/SOWCharacter.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"

#include "DataAsset/DA_StartupDataBase.h"
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

void ASOWCharacter::PossessedBy(AController* NewController)
{
	
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!StartupData.IsNull(), TEXT("Forgot to assign start up data for %s"), *GetName());

		if (!StartupData.IsNull()) {
			if (UDA_StartupDataBase* Startup = StartupData.LoadSynchronous()) {
				Startup->GiveToAbilitySystemComponent(AbilitySystemComponent);
			}
		}
	}
}
