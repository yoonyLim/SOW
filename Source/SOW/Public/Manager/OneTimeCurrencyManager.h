// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWEnumTypes.h"

class UCurrencyGainLogger;

#include "OneTimeCurrencyManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOneTiemCurrencyChanged, int32, NewCurrency, EElementalType, CurrencyType);

/**
 * 
 */
UCLASS()
class SOW_API UOneTimeCurrencyManager : public UObject
{
	GENERATED_BODY()
	
public:

	void Initialize();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Currency")
	FOnOneTiemCurrencyChanged OnOneTimeCurrencyChanged;

	UFUNCTION(BlueprintCallable, Category = "Currency")
	int32 GetCurrency(EElementalType CurrencyType) const { return CurrentCurrency[CurrencyType]; }

	UFUNCTION(BlueprintCallable, Category = "Currency")
	bool AddCurrency(EElementalType CurrencyType, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	bool SpentCurrency(EElementalType CurrencyType, int32 Amount);

	// 새 오버로드: 소스/컨텍스트 포함 (BP에서도 사용 가능)
	UFUNCTION(BlueprintCallable, Category = "Currency")
	bool AddCurrencyWithSource(EElementalType CurrencyType, int32 Amount, FName SourceTag, const FString& Context);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Currency")
	TMap<EElementalType, int32> CurrentCurrency;
	
private:
	UPROPERTY()
	UCurrencyGainLogger* GainLogger = nullptr;
};
