// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWEnumTypes.h"
#include "SkillTreeWidget.generated.h"

class UButton;
class UTextBlock;
class UCanvasPanel;
class UGlobalCurrencyManager;

/**
 * 
 */
UCLASS()
class SOW_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Currency", meta = (BindWidget))
	UTextBlock* TXT_CurrencyAmount;

	UPROPERTY(EditDefaultsOnly, Category = "PopUp", meta = (BindWidget))
	UTextBlock* TXT_ConfirmationMSG;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (BindWidget))
	UButton* BTN_Unlock;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (BindWidget))
	UButton* BTN_UnlockCancel;

	UPROPERTY(EditDefaultsOnly, Category = "PopUp", meta = (BindWidget))
	UCanvasPanel* PopUpWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Currency")
	EElementalType ElementalType;

	UGlobalCurrencyManager* GCM;

	UFUNCTION(BlueprintCallable)
	void UpdateCurrency(int32 NewCurrency, EElementalType CurrencyType);

protected:
	virtual void NativeConstruct();
};
