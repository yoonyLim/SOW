// CurrencyGainLogger.h

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SOWEnumTypes.h"
#include "CurrencyGainLogger.generated.h"

UCLASS()
class SOW_API UCurrencyGainLogger : public UObject
{
	GENERATED_BODY()
public:
	void Init();
	void LogGain(EElementalType Type, int32 Amount, const FName& Source = NAME_None, const FString& Context = TEXT(""));

private:
	FCriticalSection Mutex;
	FString FilePath;
	void AppendLine(const FString& Line);
};
