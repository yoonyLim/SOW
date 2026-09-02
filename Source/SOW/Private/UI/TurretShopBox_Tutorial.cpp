// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TurretShopBox_Tutorial.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI/TurretShopBox.h"

#include "Manager/SummonManager.h"
#include "GameModes/WaveGameMode.h"
#include "SOWGameInstance.h"

#include "Utilities/GatchaRNG.h"

#include "DataAsset/DA_SummonData.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

void UTurretShopBox_Tutorial::NativeConstruct()
{
	Super::NativeConstruct();

	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	SM = GI->GetSummonManager();
	WGM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	ShopBoxes = { ShopBox1, ShopBox2, ShopBox3, ShopBox4 };

	/*Binding*/
	/*for (UTurretShopBox* ShopBox : ShopBoxes)
	{
		ShopBox->OnBuyTurret.AddDynamic(this, &UTurretShop::RenewShopRank);
	}*/

	WGM->OnOneTimeCurrencyChanged.AddDynamic(this, &UTurretShopBox_Tutorial::UpdateCurrency);

	BTN_Reload->OnClicked.AddDynamic(this, &UTurretShopBox_Tutorial::ReLoadTurret);

	ShopBox5->SetVisibility(ESlateVisibility::Collapsed);

	/*Init*/
	UpdateCurrency(WGM->GetCurrency());
	UpdateSummonProbText();
	UpdateReloadCost();
	SetTurretByStep(ShopStep++);
}

void UTurretShopBox_Tutorial::SetTurretByStep(int32 step)
{
	if (step % 3 == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			ShopBoxes[i]->InitWidget(FirstTurretCombination[i]);
		}
	}
	else if (step % 3 == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			ShopBoxes[i]->InitWidget(SecondTurretCombination[i]);
		}
	}
	else if (step % 3 == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			ShopBoxes[i]->InitWidget(ThirdTurretCombination[i]);
		}
	}
}

void UTurretShopBox_Tutorial::UpdateReloadCost()
{
	int32 Value = DA_SummonData->GetReloadCostByShopRank(ShopRank);
	FText CostText = FText::FromString(FString::Printf(TEXT("- %d"), Value));

	TXT_RerollCost->SetText(CostText);
}

void UTurretShopBox_Tutorial::UpdateCurrency(int32 Currency)
{
	TXT_CurrentCurrency->SetText(FText::AsNumber(Currency));
}

void UTurretShopBox_Tutorial::RenewShopRank()
{
	if (bActivateOriginShop)
	{
		bActivateOriginShop = false;
	}
	ShopRank = 1;
	bTryShopRankUp = false;
}

bool UTurretShopBox_Tutorial::CheckGold(int Cost)
{
	return WGM->GetCurrency() < Cost;
}

void UTurretShopBox_Tutorial::ReLoadTurret()
{
	if (bActivateOriginShop) return;

	if (CheckGold(DA_SummonData->GetReloadCostByShopRank(ShopRank))) return;

	WGM->AddCurrency(-(DA_SummonData->GetReloadCostByShopRank(ShopRank)));

	SetTurretByStep(ShopStep++);

	UpdateSummonProbText();
	UpdateReloadCost();
}

void UTurretShopBox_Tutorial::RNGTurret()
{
	TArray<int32> TurretRarityWeights = { DA_SummonData->GetProbByRarity(ShopRank, ERarity::Common),
							DA_SummonData->GetProbByRarity(ShopRank, ERarity::Rare),
							DA_SummonData->GetProbByRarity(ShopRank, ERarity::Epic),
							DA_SummonData->GetProbByRarity(ShopRank, ERarity::Legendary),
							DA_SummonData->GetProbByRarity(ShopRank, ERarity::Origin) }; // {common, rare, epic}


	for (int i = 0; i < 4; i++)
	{
		FSummonData TurretData = SM->RNG(TurretRarityWeights);
		ShopBoxes[i]->InitWidget(TurretData);
	}
}

void UTurretShopBox_Tutorial::OpenOriginShop()
{
	//for (UTurretShopBox* ShopBox : ShopBoxes)
	//{
	//	ShopBox->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//FSummonData TurretData = SM->RNGOriginTurret();
	//ShopBox5->InitWidget(TurretData);

	//ShopBox5->SetVisibility(ESlateVisibility::Visible);

	//TXT_RerollCost->SetText(FText::FromString("-"));
}

void UTurretShopBox_Tutorial::CloseOriginShop()
{
	for (UTurretShopBox* ShopBox : ShopBoxes)
	{
		ShopBox->SetVisibility(ESlateVisibility::Visible);
	}

	ShopBox5->SetVisibility(ESlateVisibility::Collapsed);
}

bool UTurretShopBox_Tutorial::TryShopRankUp()
{
	if (ShopRank == 6)
	{
		return false;
	}

	DA_SummonData->GetProbByShopRank(ShopRank);

	TArray<int32> RankUpWeights = { DA_SummonData->GetProbByShopRank(ShopRank),
									(100 - DA_SummonData->GetProbByShopRank(ShopRank)) };

	int32 Result = GachaRNG::DrawWeightedIndex(RankUpWeights);

	switch (Result)
	{
	case 0:
		SuccessToRankUp();
		return true;
	case 1:
		break;
	}
	return false;
}

void UTurretShopBox_Tutorial::SuccessToRankUp()
{
	ShopRank += 1;
}

void UTurretShopBox_Tutorial::UpdateSummonProbText()
{
	int32 CommonValue = DA_SummonData->GetProbByRarity(ShopRank, ERarity::Common);
	FText CommonPercentText = FText::FromString(FString::Printf(TEXT("%d%%"), CommonValue));
	TXT_CommonProb->SetText(CommonPercentText);

	int32 RareValue = DA_SummonData->GetProbByRarity(ShopRank, ERarity::Rare);
	FText RarePercentText = FText::FromString(FString::Printf(TEXT("%d%%"), RareValue));
	TXT_RareProb->SetText(RarePercentText);

	int32 EpicValue = DA_SummonData->GetProbByRarity(ShopRank, ERarity::Epic);
	FText EpicPercentText = FText::FromString(FString::Printf(TEXT("%d%%"), EpicValue));
	TXT_EpicProb->SetText(EpicPercentText);

	int32 LegendaryValue = DA_SummonData->GetProbByRarity(ShopRank, ERarity::Legendary);
	FText LegendaryPercentText = FText::FromString(FString::Printf(TEXT("%d%%"), LegendaryValue));
	TXT_LegendaryProb->SetText(LegendaryPercentText);

	int32 OriginValue = DA_SummonData->GetProbByRarity(ShopRank, ERarity::Origin);
	FText OriginPercentText = FText::FromString(FString::Printf(TEXT("%d%%"), OriginValue));
	TXT_OriginProb->SetText(OriginPercentText);
}

/* Console */
static FAutoConsoleCommand Cmd_OpenOriginShopTutorial(
	TEXT("OpenOriginShopTutorial"),
	TEXT("Set OriginShop Condition and Run OpenOriginShopTutorial"),
	FConsoleCommandDelegate::CreateStatic([]()
		{
			for (TObjectIterator<UTurretShopBox_Tutorial> It; It; ++It)
			{
				UTurretShopBox_Tutorial* Shop = *It;
				if (IsValid(Shop))
				{
					Shop->ShopRank = 6;
					Shop->bTryShopRankUp = true;
					Shop->ReLoadTurret();
				}
			}
		})
);