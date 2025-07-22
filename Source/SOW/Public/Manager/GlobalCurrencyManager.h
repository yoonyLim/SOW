// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWEnumTypes.h"
#include "GlobalCurrencyManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyChanged, int32, NewCurrency, ECurrencyType, CurrencyType);

/**
 * 
 */
UCLASS()
class SOW_API UGlobalCurrencyManager : public UObject
{
	GENERATED_BODY()
	
public:

    void Initialize();

    UPROPERTY(BlueprintAssignable, Category = "Currency")
    FOnCurrencyChanged OnCurrencyChanged;

    UFUNCTION(BlueprintCallable, Category = "Currency")
    int32 GetCurrency(ECurrencyType CurrencyType) const { return CurrentCurrency[CurrencyType]; }

    UFUNCTION(BlueprintCallable, Category = "Currency")
    bool AddCurrency(ECurrencyType CurrencyType, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Currency")
    bool SpentCurrency(ECurrencyType CurrencyType, int32 Amount);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Currency")
    TMap<ECurrencyType, int32> CurrentCurrency;
};
