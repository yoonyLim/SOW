// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SummonWidget.generated.h"

/**
 * 
 */

class UButton;
class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class SOW_API USummonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UImage* IMG_MagicCircle;

	UMaterialInstanceDynamic* RoundProgressBarInst;

	UButton* BTN_FirstTurret;
	UButton* BTN_SecondTurret;
	UButton* BTN_ThirdTurret;
};
