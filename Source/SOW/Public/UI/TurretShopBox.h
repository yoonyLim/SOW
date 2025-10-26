// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/SummonManager.h"
#include "TurretShopBox.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UDA_TurretTextureData;
class UDA_SummonData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuyTurret);

/**
 * 
 */
UCLASS()
class SOW_API UTurretShopBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Property */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* BTN_Summon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TXT_TurretName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TXT_TurretCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* IMG_ElementIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* IMG_AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* IMG_RarityFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDA_TurretTextureData* DA_TextureData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSummonData TurretData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDA_SummonData* DA_SummonData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SummonCost;

	FOnBuyTurret OnBuyTurret;

public:
	UFUNCTION(BlueprintCallable)
	void InitWidget(FSummonData Turret);

	UFUNCTION(BlueprintCallable)
	void TrySummonTurret();

	UFUNCTION(BlueprintCallable)
	void HideWidget();

private:
	int8 GetCostbyRank(ERarity Rarity);

protected:
	void NativeConstruct();
};
