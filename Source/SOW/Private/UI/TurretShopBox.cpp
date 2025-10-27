// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TurretShopBox.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "DataAsset/DA_TurretTextureData.h"
#include "DataAsset/DA_SummonData.h"

#include "SOWGameInstance.h"
#include "Manager/SummonManager.h"

#include "GameModes/WaveGameMode.h"

#include "Kismet/GameplayStatics.h"

void UTurretShopBox::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Summon)
	{
		BTN_Summon->OnClicked.AddDynamic(this, &UTurretShopBox::TrySummonTurret);
	}
}

void UTurretShopBox::InitWidget(FSummonData Turret)
{
	SetVisibility(ESlateVisibility::Visible);

	if (DA_TextureData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no TextureData"));
	}

	TurretData = Turret;

	TXT_TurretName->SetText(FText::FromName(TurretData.TurretName));
	SummonCost = GetCostbyRank(TurretData.Rarity);
	TXT_TurretCost->SetText(FText::AsNumber(SummonCost));


	UE_LOG(LogTemp, Warning, TEXT("InitWidget: AttackType = %d"), (int32)TurretData.TurretAttackType);
	IMG_AttackType->SetBrushFromTexture(DA_TextureData->GetAttackTypeTexture(TurretData.TurretAttackType));
	IMG_ElementIcon->SetBrushFromTexture(DA_TextureData->GetElementTexture(TurretData.ElementalType));

	BTN_Summon->WidgetStyle.Normal.SetResourceObject(TurretData.ProfileTexture);
	BTN_Summon->WidgetStyle.Hovered.SetResourceObject(TurretData.ProfileTexture);
	BTN_Summon->WidgetStyle.Pressed.SetResourceObject(TurretData.ProfileTexture);

	IMG_RarityFrame->SetBrushFromTexture(DA_TextureData->GetShopBoxFrameByRarity(TurretData.Rarity));
}

void UTurretShopBox::TrySummonTurret()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (GM->GetCurrency() < SummonCost)
	{
		return;
	}

	GM->AddCurrency(-(SummonCost));

	USummonManager* SM = GI->GetSummonManager();

	SM->TurretSummon(TurretData);
	OnBuyTurret.Broadcast();

	HideWidget();
}

void UTurretShopBox::HideWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

int8 UTurretShopBox::GetCostbyRank(ERarity Rarity)
{
	SummonCost = DA_SummonData->GetCostByRarity(Rarity);
	return SummonCost;
}