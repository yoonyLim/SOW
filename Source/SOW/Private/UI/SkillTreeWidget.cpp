// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillTreeWidget.h"
#include "SOWGameInstance.h"
#include "Manager/GlobalCurrencyManager.h"
#include "Components/TextBlock.h"

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());

	GCM = GI->GetGlobalCurrencyManager();

	GCM->OnCurrencyChanged.AddDynamic(this, &USkillTreeWidget::UpdateCurrency);

	if (TXT_CurrencyAmount)
	{
		FText CurrencyAmount = FText::AsNumber(GCM->GetCurrency(ElementalType));

		TXT_CurrencyAmount->SetText(CurrencyAmount);
	}
}

void USkillTreeWidget::UpdateCurrency(int32 NewCurrency, EElementalType CurrencyType)
{
	if (CurrencyType == ElementalType)
	{
		if (TXT_CurrencyAmount)
		{
			FText CurrencyAmount = FText::AsNumber(NewCurrency);

			TXT_CurrencyAmount->SetText(CurrencyAmount);
		}
	}
}