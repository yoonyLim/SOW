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
#include "Manager/SummonManager.h"
#include "SOWGameInstance.h"
#include "Core/SOWPlayerController.h"
#include "Kismet/GameplayStatics.h"


void USummonWidget::NativeConstruct()
{
	Super::NativeConstruct();


	DT_SummonTurretProb = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/01Blueprints/DataTable/Turrets/DT_TurretSummonProb.DT_TurretSummonProb")
	);

	if (!DT_SummonTurretProb)
	{
		UE_LOG(LogTemp, Error, TEXT("SummongWidget: Failed to load Turret Summon Prob DataTable"));
	}

	if (BTN_Summon) BTN_Summon->OnClicked.AddDynamic(this, &USummonWidget::SummonTurret);
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


