// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/SummonManager.h"
#include "SummonNotificationWidget.generated.h"

class UTextBlock;
class UTexture2D;
class UImage;
class UWidgetAnimation;
class UMaterialInterface;
class UMaterialInstanceDynamic;

DECLARE_MULTICAST_DELEGATE(FOnNotifyFinished);

 /* */
UCLASS()
class SOW_API USummonNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FTimerHandle CleanupHandle;

	FOnNotifyFinished OnNotifyFinished;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TXT_Info;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Icon;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	UTexture2D* Icon_Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	UTexture2D* Icon_Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	UTexture2D* Icon_Support;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	UTexture2D* Frame_Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	UTexture2D* Frame_Rare;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	UTexture2D* Frame_Epic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Tint_Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Tint_Rare;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Tint_Epic;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* AnimIn;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* AnimOut;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* AnimFastOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float DisplaySeconds = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	UMaterialInterface* MI_Gradient = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* MID_Gradient = nullptr;


public:
	UFUNCTION(BlueprintCallable, Category = "Toast")
	void Setup(const FSummonData& TurretToSummon);

	UFUNCTION(BlueprintCallable, Category = "Toast")
	void PlayIn();

	UFUNCTION(BlueprintCallable, Category = "Toast")
	void ForceClose(bool bFast);

	bool IsClosing() const { return bClosing; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	FTimerHandle LifeHandle;
	bool bClosing = false;

	void StartLifeTimer();
	UFUNCTION() void OnLifeEnded();
	UFUNCTION() void NotifyFinished();
};
