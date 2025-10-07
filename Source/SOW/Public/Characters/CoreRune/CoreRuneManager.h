// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoreRuneManager.generated.h"

class ASOWCharacterCoreRune;
/**
 * 
 */
UCLASS()
class SOW_API UCoreRuneManager : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ASOWCharacterCoreRune* CoreRune;

	

public:
	//UFUNCTION(BlueprintCallable)
	void MakeCoreRuneReference(ASOWCharacterCoreRune* InRune);

	void Initialize();

	ASOWCharacterCoreRune* GetCoreRune() const;
	
};
