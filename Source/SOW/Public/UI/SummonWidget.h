// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWStructTypes.h"
#include "Manager/SummonManager.h"
#include "SummonWidget.generated.h"

/**
 * 
 */

class UButton;
class UDataTable;
class UToastStackWidget;

UCLASS()
class SOW_API USummonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UDataTable* DT_SummonTurretProb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon", meta = (BindWidget))
	UButton* BTN_Summon;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UToastStackWidget* W_SummonNotiBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ToastDuration = 2.0f;

public:
	UFUNCTION()
	void SummonTurret();

	UFUNCTION()
	void OnTurretSummoned(const FSummonData& TurretToSummon);

private:

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
