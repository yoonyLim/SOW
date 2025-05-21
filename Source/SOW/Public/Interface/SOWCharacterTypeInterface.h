// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SOWEnumTypes.h"
#include "SOWCharacterTypeInterface.generated.h"

// This class does not need to be modified.
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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ESOWCharacterType GetSOWCharacterType() const = 0;
};
