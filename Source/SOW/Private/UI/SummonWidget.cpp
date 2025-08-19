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

	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	USummonManager* SummonManager = GI->GetSummonManager();

	FGameplayTag NatureTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));

	CircleLevel = SummonManager->GetCircleLevel(NatureTag);

	if (CircleLevel == 0)
	{
		RemoveFromParent();
	}

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
	if (BTN_FifthSpell) BTN_FifthSpell->OnClicked.AddDynamic(this, &USummonWidget::HandleButtonClicked4);

	if (DT_MagicSpell)
	{
		static const FString ContextString(TEXT("Spell Filter"));

		for (const auto& RowPair : DT_MagicSpell->GetRowMap())
		{
			const FMagicSpell* Row = reinterpret_cast<const FMagicSpell*>(RowPair.Value);
			if (Row)
			{
				SpellList.Add(Row);

				FString InternalName = StaticEnum<EMagicSpell>()->GetNameStringByValue(static_cast<int64>(Row->MagicSpell));
				UE_LOG(LogTemp, Warning, TEXT("Internal Enum Name: %s"), *InternalName);
			}
		}
	}
	for (int32 i = 0; i < 5; i++)
	{
		if (SpellButtonArray[i])
		{
			SpellButtonArray[i]->SetVisibility(ESlateVisibility::Visible);
			SetButtonStyle(SpellButtonArray[i], EElementalType::Nature, false);
		}
	}

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
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	USummonManager* SummonManager = GI->GetSummonManager();

	FGameplayTag NatureTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));

	if (RoundProgressBarInst)
	{
		switch (SummonManager->GetCircle(NatureTag))
		{
		case 0:
			break;
		case 1:
		{
			RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv1);
			break;
		}
		case 2:
		{
			RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv2);
			break;
		}
		case 3:
		{
			RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv3);
			break;
		}
		case 4:
		{
			RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv4);
			break;
		}
		case 5:
		{
			RoundProgressBarInst->SetTextureParameterValue(FName("Texture"), MagicCircleTexture_Lv5);
			break;
		}
		}
	}
	return;
}

void USummonWidget::SetMagicSpell()
{
	if (CircleLevel == 0) return;

	PlayAnimation(SpellFadeIn);
}

void USummonWidget::OnIndexedButtonClicked(uint8 Index)
{

	SelectedSpells.Add(SpellList[Index]);


	FString InternalName = StaticEnum<EMagicSpell>()->GetNameStringByValue(static_cast<int64>(SpellList[Index]->MagicSpell));
	UE_LOG(LogTemp, Warning, TEXT("Internal Enum Name: %s"), *InternalName);

	UE_LOG(LogTemp, Error, TEXT("SummonWidget: SpellList Num is  %d"), Index);
	L_SelectedSpellsIndex.Add(Index);

	OnDelayFinished();
}


void USummonWidget::SetButtonStyle(UButton* TargetButton, EElementalType ElementalType ,bool bCanBeComp)
{
	if (!TargetButton)
		return;

	FButtonStyle ButtonStyle = TargetButton->GetStyle();

	if (bCanBeComp)
	{
		ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(72.f / 255.f, 156.f / 255.f, 20.f / 255.f, 1.f));
		ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(72.f / 255.f, 156.f / 255.f, 20.f / 255.f, 1.f));
		ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(72.f / 255.f, 156.f / 255.f, 20.f / 255.f, 1.f));
	}
	else
	{
		ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
		ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor::White);
		ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor::White);
	}


	TargetButton->SetStyle(ButtonStyle);
}


void USummonWidget::OnDelayFinished()
{
	UE_LOG(LogTemp, Error, TEXT("SummonWidget: %d"), CurrentStep);

	if (CurrentStep == 1)
	{
		HighlightMagicSpell();
		CurrentStep += 1;
		IMG_FirstSpell->SetBrushFromTexture(SelectedSpells[0]->SanskritImage);
		IMG_FirstSpell->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	else if (CurrentStep == 2)
	{
		HighlightMagicSpell();
		CurrentStep += 1;
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

void USummonWidget::HighlightMagicSpell()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	USummonManager* SummonManager = GI->GetSummonManager();

	EElementalType ET = EElementalType::Nature;

	TMap<uint8, TArray<uint8>>* M_SpellComp= SummonManager->GetSpellCompMap(ET, CurrentStep);

	uint8 Key = IndexToKey();

	UE_LOG(LogTemp, Warning, TEXT("Index Key String: %d"), Key);

	if (!(*M_SpellComp).Contains(Key))
	{
		UE_LOG(LogTemp, Error, TEXT("Here"));
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		if ((*M_SpellComp)[Key].Contains(i))
		{
			SetButtonStyle(SpellButtonArray[i], EElementalType::Nature, true);
		}
		else
		{
			SetButtonStyle(SpellButtonArray[i], EElementalType::Nature, false);
		}
	}
}

uint8 USummonWidget::IndexToKey()
{
	FString A = "";

	for (uint8 Num : L_SelectedSpellsIndex)
	{
		A += FString::FormatAsNumber(Num);
	}

	return FCString::Atoi(*A);
}

uint8 USummonWidget::GetCircleLevel()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	USummonManager* SummonManager = GI->GetSummonManager();

	FGameplayTag NatureTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));

	return SummonManager->GetCircleLevel(NatureTag);
}