// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWEnumTypes.h"
#include "GlobalCurrencyManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyChanged, int32, NewCurrency, EElementalType, CurrencyType);

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
    int32 GetCurrency(EElementalType CurrencyType) const { return CurrentCurrency[CurrencyType]; }

    UFUNCTION(BlueprintCallable, Category = "Currency")
    bool AddCurrency(EElementalType CurrencyType, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Currency")
    bool SpentCurrency(EElementalType CurrencyType, int32 Amount);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Currency")
    TMap<EElementalType, int32> CurrentCurrency;
};
