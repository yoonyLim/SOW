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
class UImage;
class UDataTable;
class UTexture2D;
class UMaterialInstanceDynamic;
class UWidgetAnimation;

UCLASS()
class SOW_API USummonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon", meta = (BindWidget))
	UImage* IMG_MagicCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon")
	UMaterialInterface* BaseProgressMaterial;

	UMaterialInstanceDynamic* RoundProgressBarInst;

	UPROPERTY()
	UDataTable* DT_MagicSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Visual")
	UTexture2D* MagicCircleTexture_Lv1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Visual")
	UTexture2D* MagicCircleTexture_Lv2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Visual")
	UTexture2D* MagicCircleTexture_Lv3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Visual")
	UTexture2D* MagicCircleTexture_Lv4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UButton* BTN_FirstSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UButton* BTN_SecondSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UButton* BTN_ThirdSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UButton* BTN_FourthSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UButton* BTN_FifthSpell;

	TArray<UButton*> SpellButtonArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon", meta = (ExposeOnSpawn))
	uint8 CircleLevel;

	float Percent = 0.0f;

	uint8 CurrentStep = 1;

	TArray<const FMagicSpell*> SpellList;

	TArray<const FMagicSpell*> SelectedSpells;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SpellFadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SpellFadeOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UImage* IMG_FirstSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UImage* IMG_SecondSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon|Spells", meta = (BindWidget))
	UImage* IMG_ThirdSpell;


public:
	UFUNCTION()
	void OnDelayFinished();

private:
	UFUNCTION(BlueprintCallable)
	void SetImage();

	UFUNCTION(BlueprintCallable)
	void SetPercent();

	UFUNCTION(BlueprintCallable)
	void SetMagicSpell();

	UFUNCTION(BlueprintCallable)
	void PlaceTurret();

	UFUNCTION(BlueprintCallable)
	void SetButtonStyle(UButton* TargetButton, UTexture2D* TargetImage);

	TArray<int32> GetUniqueRandomNumbers(int32 Min, int32 Max, int32 Count);

	UFUNCTION()
	void HandleButtonClicked0() { OnIndexedButtonClicked(0); }

	UFUNCTION()
	void HandleButtonClicked1() { OnIndexedButtonClicked(1); }

	UFUNCTION()
	void HandleButtonClicked2() { OnIndexedButtonClicked(2); }

	UFUNCTION()
	void HandleButtonClicked3() { OnIndexedButtonClicked(3); }

	UFUNCTION()
	void HandleButtonClicked4() { OnIndexedButtonClicked(4); }

	UFUNCTION(BlueprintCallable)
	void OnIndexedButtonClicked(int32 Index);


protected:
	virtual void NativeConstruct();
	virtual void NativePreConstruct();
};
