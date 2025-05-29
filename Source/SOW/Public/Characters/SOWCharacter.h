// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SOWEnumTypes.h"
#include "GameplayEffectTypes.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWCharacter.generated.h"

class USOWAbilitySystemComponent;
class USOWAttributeSet;
class UDA_StartupDataBase;

//struct FOnAttributeChangeData;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASOWCharacter : public ACharacter, public ISOWCharacterTypeInterface
{
	GENERATED_BODY()

public:
	ASOWCharacter();

	/* Begin ISOWCharacterTypeInterface implement */
	virtual ESOWCharacterType GetSOWCharacterType() const override;
	/* End ISOWCharacterTypeInterface implement */

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "AbilitySystem")
	USOWAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	USOWAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDA_StartupDataBase> StartupData;

	ESOWCharacterType CharacterType;
	

	void OnWalkSpeedChanged(const FOnAttributeChangeData& Data);
	
};
