// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SummonWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "SOWStructTypes.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateColor.h"
#include "Slate/SlateBrushAsset.h"
#include "UObject/UObjectGlobals.h" 
#include "Animation/WidgetAnimation.h"
#include "TimerManager.h"
#include "SOWGameInstance.h"
#include "Core/SOWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ToastStackWidget.h"


void USummonWidget::NativeConstruct()
{
	Super::NativeConstruct();


	DT_SummonTurretProb = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/01Blueprints/DataTable/Turrets/DT_TurretSummonProb_Implemented.DT_TurretSummonProb_Implemented") // Testable Turret Only
	);

	if (!DT_SummonTurretProb)
	{
		UE_LOG(LogTemp, Error, TEXT("SummongWidget: Failed to load Turret Summon Prob DataTable"));
	}

	if (BTN_Summon) BTN_Summon->OnClicked.AddDynamic(this, &USummonWidget::SummonTurret);

	W_SummonNotiBox->OnBecameEmpty.AddLambda([this]()
		{
			W_SummonNotiBox->SetVisibility(ESlateVisibility::Collapsed);
		});

	if (USummonManager* SM = Cast<USOWGameInstance>(GetGameInstance())->GetSummonManager())
	{
		SM->OnSummonTurret.AddDynamic(this, &USummonWidget::OnTurretSummoned);
	}
}

void USummonWidget::NativeDestruct()
{
	if (USOWGameInstance* GI = Cast<USOWGameInstance>(GetGameInstance()))
	{
		if (USummonManager* SM = GI->GetSummonManager())
		{
			SM->OnSummonTurret.RemoveDynamic(this, &USummonWidget::OnTurretSummoned);
		}
	}

	Super::NativeDestruct();
}

void  USummonWidget::SummonTurret()
{
	if (Cast<USOWGameInstance>(GetWorld()->GetGameInstance())->GetSummonManager()->TurretSummon())
	{
		UE_LOG(LogTemp, Error, TEXT("SummongWidget: Success to Summon Turret"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SummongWidget: Fail to Summon Turret"));
	}
}

void USummonWidget::OnTurretSummoned(const FSummonData& TurretToSummon)
{
	if (!W_SummonNotiBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SummongWidget: No SummonNotiBox"));
		return;
	}

	if (W_SummonNotiBox->GetVisibility() == ESlateVisibility::Collapsed)
	{
		W_SummonNotiBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	W_SummonNotiBox->PushToast(TurretToSummon);
}


