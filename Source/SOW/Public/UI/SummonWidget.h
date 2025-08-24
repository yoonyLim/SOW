// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWStructTypes.h"
#include "SummonWidget.generated.h"

/**
 * 
 */

class UButton;
class UDataTable;

UCLASS()
class SOW_API USummonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UDataTable* DT_SummonTurretProb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon", meta = (BindWidget))
	UButton* BTN_Summon;

public:
	UFUNCTION()
	void SummonTurret();

private:

protected:
	virtual void NativeConstruct();
};
