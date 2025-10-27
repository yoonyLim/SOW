// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/TurretShopBox.h"
#include "TurretShop.generated.h"

class UButton;
class UTextBlock;
class UDA_SummonData;
class USummonManager;
class AWaveGameMode;

/**
 * 
 */
UCLASS()
class SOW_API UTurretShop : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_RerollCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_CurrentCurrency;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_CommonProb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_RareProb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_EpicProb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_LegendaryProb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_OriginProb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTurretShopBox* ShopBox1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTurretShopBox* ShopBox2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTurretShopBox* ShopBox3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTurretShopBox* ShopBox4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTurretShopBox* ShopBox5;

	TArray<UTurretShopBox*> ShopBoxes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDA_SummonData* DA_SummonData;

	USummonManager* SM;

	AWaveGameMode* WGM;

	/* Property */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int ShopRank = 1;

	bool bTryShopRankUp = false;

	bool bActivateOriginShop = false;

public:
	UFUNCTION(BlueprintCallable)
	void ReLoadTurret();

	UFUNCTION(BlueprintCallable)
	bool TryShopRankUp();

	UFUNCTION(BlueprintCallable)
	void UpdateSummonProbText();

	UFUNCTION(BlueprintCallable)
	void SuccessToRankUp();
	
	UFUNCTION(BlueprintCallable)
	bool CheckGold(int Cost);

	UFUNCTION(BlueprintCallable)
	void RenewShopRank();

	UFUNCTION(BlueprintCallable)
	void UpdateCurrency(int32 TotalCurrency);

	UFUNCTION(BlueprintCallable)
	void UpdateReloadCost();

	UFUNCTION(BlueprintCallable)
	void OpenOriginShop();

	UFUNCTION(BlueprintCallable)
	void CloseOriginShop();

	UFUNCTION(BlueprintCallable)
	void RNGTurret();

protected:
	void NativeConstruct();
};
