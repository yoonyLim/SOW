// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"
#include "Components/ActorComponent.h"
#include "SOWTurretEvolutionComponent.generated.h"

class ASOWCharacterTurretBase;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWTurretEvolutionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWTurretEvolutionComponent();

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	int32 GetTurretEvolutionLevel() const { return EvolutionLevel; }

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	void GetTurretEvolutionDescriptions(FString& AlphaDesc, FString& BetaDesc);

	UFUNCTION(BlueprintCallable, Category = "Turret|Evolution")
	EEvolutionType GetTurretEvolutionType() const { return EvolutionState; }


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int32 EvolutionLevel = 0;
	EEvolutionType EvolutionState = EEvolutionType::EVO_NONE;


private:
	ASOWCharacterTurretBase* CachedOwnerCharacter;
	FTurretEvolutionData* EvolutionDataRow;

	FTurretEvolutionItem Alpha;
	FTurretEvolutionItem Beta;

	UFUNCTION()
	void TryEvolution(EEvolutionType Type);

	void InitEvolutionItems();
};
