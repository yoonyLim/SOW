// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SummonNotificationWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

void USummonNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IMG_Icon && MI_Gradient && !MID_Gradient)
	{
		MID_Gradient = UMaterialInstanceDynamic::Create(MI_Gradient, this);
		IMG_Icon->SetBrushFromMaterial(MID_Gradient);
	}
}

void USummonNotificationWidget::Setup(const FSummonData& TurretToSummon)
{
	if (MI_Gradient && !MID_Gradient)
	{
		MID_Gradient = UMaterialInstanceDynamic::Create(MI_Gradient, this);
		IMG_Icon->SetBrushFromMaterial(MID_Gradient);
	}

	FString Rarity;

	if (!TXT_Info) return;
	if (!IMG_Icon) return;
	if (!IMG_Frame) return;

	switch (TurretToSummon.Rarity)
	{
	case ERarity::Common:
		Rarity = TEXT("일반");
		MID_Gradient->SetVectorParameterValue(TEXT("Color"), Tint_Common);
		IMG_Frame->SetBrushFromTexture(Frame_Common);
		break;
	case ERarity::Rare:
		Rarity = TEXT("레어");
		MID_Gradient->SetVectorParameterValue(TEXT("Color"), Tint_Rare);
		IMG_Frame->SetBrushFromTexture(Frame_Rare);
		break;
	case ERarity::Epic:
		Rarity = TEXT("에픽");
		MID_Gradient->SetVectorParameterValue(TEXT("Color"), Tint_Epic);
		IMG_Frame->SetBrushFromTexture(Frame_Epic);
		break;
	}

	switch (TurretToSummon.TurretAttackType)
	{
	case ETurretAttackType::Melee:
		MID_Gradient->SetTextureParameterValue(TEXT("Icon"), Icon_Melee);
		break;
	case ETurretAttackType::Range:
		MID_Gradient->SetTextureParameterValue(TEXT("Icon"), Icon_Range);
		break;
	case ETurretAttackType::Support:
		MID_Gradient->SetTextureParameterValue(TEXT("Icon"), Icon_Support);
		break;
	}

	const FText Line = FText::FromString(
		FString::Printf(TEXT("%s : %s"), *Rarity, *TurretToSummon.TurretName.ToString())
	);

	TXT_Info->SetText(Line);
}

void USummonNotificationWidget::PlayIn()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] PlayIn life=%.2f"), *GetName(), DisplaySeconds);

	if (AnimIn) PlayAnimation(AnimIn);
	StartLifeTimer();
}

void USummonNotificationWidget::ForceClose(bool bFast)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] ForceClose(bFast=%d) Out=%p FastOut=%p"), *GetName(), bFast ? 1 : 0, AnimOut, AnimFastOut);

	if (bClosing) return;
	bClosing = true;

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(LifeHandle);
	}

	FWidgetAnimationDynamicEvent FinishedEvent;
	FinishedEvent.BindDynamic(this, &USummonNotificationWidget::NotifyFinished);

	if (bFast && AnimFastOut)
	{
		BindToAnimationFinished(AnimFastOut, FinishedEvent);
		PlayAnimation(AnimFastOut);
	}
	
	else if (AnimOut)
	{
		BindToAnimationFinished(AnimOut, FinishedEvent);
		PlayAnimation(AnimOut);
	}
	else
	{
		NotifyFinished();
	}
}

void USummonNotificationWidget::NativeDestruct()
{
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(LifeHandle);
	}
	Super::NativeDestruct();
}

void USummonNotificationWidget::StartLifeTimer()
{
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().SetTimer(LifeHandle, this, &USummonNotificationWidget::OnLifeEnded, FMath::Max(0.0f, DisplaySeconds), false);

	}
}

void USummonNotificationWidget::OnLifeEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] OnLifeEnded -> ForceClose(false)"), *GetName());
	ForceClose(false);
}

void USummonNotificationWidget::NotifyFinished()
{
	OnNotifyFinished.Broadcast();
}