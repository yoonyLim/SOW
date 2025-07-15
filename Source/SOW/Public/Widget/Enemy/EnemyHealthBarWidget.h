// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

	FTimerHandle HealthBarEffectHandle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* SecondaryHealthBar = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	class UWidgetAnimation* FadeInOutAnimation;

public:
	UEnemyHealthBarWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetHealthBarPercent(float const Value);
	void HideInGame(bool bHidden);
	FVector2D GetHealthBarSize();
	void PlayFadeAnimation();
};
