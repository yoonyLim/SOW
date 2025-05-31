// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SOWEnumTypes.h"
#include "SOWCharacterTypeInterface.generated.h"

// added by PGH
// mix-in target -> SOWCharacter
// purpose : check character type : player / turret / enemy

// every character needs to initialize Character type variable (enum)

UINTERFACE(MinimalAPI)
class USOWCharacterTypeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOW_API ISOWCharacterTypeInterface
{
	GENERATED_BODY()


public:

	virtual ESOWCharacterType GetSOWCharacterType() const = 0;
};
