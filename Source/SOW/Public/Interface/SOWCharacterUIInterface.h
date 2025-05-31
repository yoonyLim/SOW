// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SOWCharacterUIInterface.generated.h"

class USOWCharacterUIComponent;

// added by PGH
// mix-in target -> SOWCharacter
// purpose : get UI Component that listen delegates and execute callbacks 

// if characters have own UI Component, override GetCharacterUIComponent() in their base code.


UINTERFACE(MinimalAPI)
class USOWCharacterUIInterface : public UInterface
{
	GENERATED_BODY()
};
/**
 * 
 */
class SOW_API ISOWCharacterUIInterface
{
	GENERATED_BODY()

public:

	virtual USOWCharacterUIComponent* GetCharacterUIComponent() const = 0;
};
