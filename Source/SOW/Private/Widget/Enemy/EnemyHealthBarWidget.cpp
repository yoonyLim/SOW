// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Enemy/EnemyHealthBarWidget.h"

UEnemyHealthBarWidget::UEnemyHealthBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEnemyHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HealthBar->GetPercent() < SecondaryHealthBar->GetPercent())
		SecondaryHealthBar->SetPercent(FMath::Lerp(SecondaryHealthBar->GetPercent(), HealthBar->GetPercent(), 0.1f));
}

void UEnemyHealthBarWidget::SetHealthBarPercent(float const Value)
{
	HealthBar->SetPercent(Value);
}

void UEnemyHealthBarWidget::HideInGame(bool bHidden)
{
	if (bHidden)
		SetVisibility(ESlateVisibility::Hidden);
	else
		SetVisibility(ESlateVisibility::Visible);
}

FVector2D UEnemyHealthBarWidget::GetHealthBarSize()
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
	return CanvasSlot->GetSize();
}

void UEnemyHealthBarWidget::PlayFadeAnimation()
{
	PlayAnimation(FadeInOutAnimation);
}