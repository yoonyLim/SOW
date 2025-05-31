// Fill out your copyright notice in the Description page of Project Settings.

// added by PGH
// mix-in target -> SOWCharacter
// purpose : Send implemented delegate to all UI Assets in WBP. 

// OnCurrentHealthChanged - every character has health attribute and need to listen health change

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWCharacterUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthPercentChangedDelegate, float, NewPercent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWCharacterUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentChangedDelegate OnCurrentHealthChanged;
};
