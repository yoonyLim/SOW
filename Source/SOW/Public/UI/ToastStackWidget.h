// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/SummonManager.h"
#include "ToastStackWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBecameEmpty);

class UVerticalBox;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class SOW_API UToastStackWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnBecameEmpty OnBecameEmpty;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UVerticalBox* VB_Toasts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxConcurrent = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USummonNotificationWidget> ToastClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoCollpaseWhenEmpty = true;

public:
	UFUNCTION(BlueprintCallable)
	void PushToast(const FSummonData& TurretToSummom);

	UFUNCTION(BlueprintCallable)
	void ClearAll(bool bFastOut = true);

protected:
	virtual void NativeConstruct() override;
};
