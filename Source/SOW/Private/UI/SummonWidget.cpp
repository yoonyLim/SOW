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
#include "Core/SOWPlayerController.h"
#include "Kismet/GameplayStatics.h"

void USummonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UMaterialInterface* MaterialFinder = LoadObject<UMaterialInterface>(
		nullptr,
		TEXT("/Game/03Materials/MI_RoundProgressBar.MI_RoundProgressBar")
	);

	if (MaterialFinder)
	{
		BaseProgressMaterial = MaterialFinder;
	}

	if (!RoundProgressBarInst && BaseProgressMaterial && IMG_MagicCircle)
	{
		RoundProgressBarInst = UMaterialInstanceDynamic::Create(BaseProgressMaterial, this);
		IMG_MagicCircle->SetBrushFromMaterial(RoundProgressBarInst);
	}
}

void USummonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DT_MagicSpell = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/01Blueprints/DataTable/MagicSpell/DT_MagicSpell.DT_MagicSpell")
	);

	if (!DT_MagicSpell)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Magic Spell DataTable"));
	}

	SpellButtonArray = { BTN_FirstSpell, BTN_SecondSpell, BTN_ThirdSpell, BTN_FourthSpell, BTN_FifthSpell };

	if (BTN_FirstSpell) BTN_FirstSpell->OnClicked.AddDynamic(this, &USummonWidget::HandleButtonClicked0);
	if (BTN_SecondSpell) BTN_SecondSpell->OnClicked.AddDynamic(this, &USummonWidget::HandleButtonClicked1);
	if (BTN_ThirdSpell) BTN_ThirdSpell->OnClicked.AddDynamic(this, &USummonWidget::HandleButtonClicked2);
	if (BTN_FourthSpell) BTN_FourthSpell->OnClicked.AddDynamic(this, &USummonWidget::HandleButtonClicked3);
	if (BTN_FifthSpell) BTN_FourthSpell->OnClicked.AddDynamic(this, &USummonWidget::HandleButtonClicked4);

	SetPercent();
	SetImage();
}

void USummonWidget::SetPercent()
{
	if (RoundProgressBarInst)
	{
		RoundProgressBarInst->SetScalarParameterValue(FName("Percent"), Percent);
	}

	return;
}

void USummonWidget::SetImage()
{
	if (RoundProgressBarInst)
	{
		if (CircleLevel)
		{
			switch (CircleLevel)
			{
			case 1:
				RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv1);
				break;
			case 2:
				RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv2);
				break;
			case 3:
				RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv3);
				break;
			case 4:
				RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv4);
				break;
			}
		}
	}

	return;
}

TArray<int32> USummonWidget::GetUniqueRandomNumbers(int32 Min, int32 Max, int32 Count)
{
	TArray<int32> Pool;
	for (int32 i = Min; i <= Max; ++i)
	{
		Pool.Add(i);
	}

	Pool.Sort([](int32 A, int32 B) { return FMath::RandBool(); }); 

	TArray<int32> Result;
	for (int32 i = 0; i < Count && i < Pool.Num(); ++i)
	{
		Result.Add(Pool[i]);
	}

	return Result;
}

void USummonWidget::SetMagicSpell()
{
	TArray<const FMagicSpell*> Spells;

	if (DT_MagicSpell)
	{
		static const FString ContextString(TEXT("Spell Filter"));

		for (const auto& RowPair : DT_MagicSpell->GetRowMap())
		{
			const FMagicSpell* Row = reinterpret_cast<const FMagicSpell*>(RowPair.Value);
			if (Row)
			{
				Spells.Add(Row);
			}
		}
		UE_LOG(LogTemp, Error, TEXT("SummonWidget: step is %d"), CurrentStep);

		UE_LOG(LogTemp, Log, TEXT("SummonWidget: spell num is %d"), Spells.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MagicSpell DataTable is NULL"));
	}

	TArray<int32> RandomInts = GetUniqueRandomNumbers(0, (Spells.Num() - 1), 4);

	UE_LOG(LogTemp, Error, TEXT("SummonWidget: %d"), RandomInts.Num());

	for (int32 i = 0; i < RandomInts.Num(); ++i)
	{
		if (SpellButtonArray[i])
		{
			SpellList.Add(Spells[RandomInts[i]]);
			SpellButtonArray[i]->SetVisibility(ESlateVisibility::Visible);
			SetButtonStyle(SpellButtonArray[i], Spells[RandomInts[i]]->SanskritImage);
		}
	}

	PlayAnimation(SpellFadeIn);
}

void USummonWidget::OnIndexedButtonClicked(int32 Index)
{
	UE_LOG(LogTemp, Error, TEXT("SummonWidget: SpellList Num is  %d"), SpellList.Num());

	SelectedSpells.Add(SpellList[Index]);
	SpellList.Empty();

	if (SpellFadeOut)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindDynamic(this, &USummonWidget::OnDelayFinished);

		// 애니메이션 바인딩
		BindToAnimationFinished(SpellFadeOut, AnimationFinishedEvent);

		// 애니메이션 재생
		PlayAnimation(SpellFadeOut);
	}
}


void USummonWidget::SetButtonStyle(UButton* TargetButton, UTexture2D* TargetImage)
{
	if (!TargetButton || !TargetImage)
		return;

	FSlateBrush TargetBrush;
	TargetBrush.SetResourceObject(TargetImage);
	TargetBrush.ImageSize = FVector2D(250.f, 150.f);

	FButtonStyle ButtonStyle;
	ButtonStyle.SetNormal(TargetBrush);
	ButtonStyle.SetHovered(TargetBrush);
	ButtonStyle.SetPressed(TargetBrush);

	ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
	ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor::White);
	ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor::White);

	TargetButton->WidgetStyle = ButtonStyle;
}


void USummonWidget::OnDelayFinished()
{
	UE_LOG(LogTemp, Error, TEXT("SummonWidget: %d"), CurrentStep);



	if (CurrentStep == 1)
	{
		CurrentStep += 1;
		SetMagicSpell();
		IMG_FirstSpell->SetBrushFromTexture(SelectedSpells[0]->SanskritImage);
		IMG_FirstSpell->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	else if (CurrentStep == 2)
	{
		CurrentStep += 1;
		SetMagicSpell();
		IMG_SecondSpell->SetBrushFromTexture(SelectedSpells[1]->SanskritImage);
		IMG_SecondSpell->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	else if (CurrentStep == 3)
	{
		if (3 == SelectedSpells.Num())
		{
			IMG_ThirdSpell->SetBrushFromTexture(SelectedSpells[2]->SanskritImage);
			IMG_ThirdSpell->SetVisibility(ESlateVisibility::Visible);
			PlaceTurret();
			return;
		}
	}
}

void USummonWidget::PlaceTurret()
{
	ASOWPlayerController* PC = Cast<ASOWPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	FSpellCombination Spells = { SelectedSpells[0]->MagicSpell , SelectedSpells[1]->MagicSpell,SelectedSpells[2]->MagicSpell};
	PC->StartPlacingTurret(Spells);

	RemoveFromParent();
}