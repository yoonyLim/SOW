// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillIconWidget.h"
#include "Components/Button.h"
#include "SOWGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/USkillManager.h"

void USkillIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_SkillIcon) BTN_SkillIcon->OnClicked.AddDynamic(this, &USkillIconWidget::OnSkillIconClicked);
}

void USkillIconWidget::OnSkillIconClicked()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(UGameplayStatics::GetGameInstance(this));
	
	UUSkillManager* SM = GI->GetSkillManager();

	if (!bIsUnlock)
	{
		if (SM->UnlockSkill(SkillID))
		{
			UnlockSkillIconWidget();
		}
		else
		{
			LockSkillIconWidget();
		}
	}
	else
	{
		return;
	}
}

void USkillIconWidget::UnlockSkillIconWidget()
{
	FButtonStyle CurrentStyle = BTN_SkillIcon->GetStyle();

	CurrentStyle.Normal.TintColor = FSlateColor(NormalColor);
	CurrentStyle.Hovered.TintColor = FSlateColor(NormalColor);
	CurrentStyle.Pressed.TintColor = FSlateColor(NormalColor);
	
	BTN_SkillIcon->SetStyle(CurrentStyle);
	bIsUnlock = true;
}

void USkillIconWidget::LockSkillIconWidget()
{
	FButtonStyle CurrentStyle = BTN_SkillIcon->GetStyle();

	CurrentStyle.Normal.TintColor = FSlateColor(FLinearColor::Gray);
	CurrentStyle.Hovered.TintColor = FSlateColor(FLinearColor::Gray);
	CurrentStyle.Pressed.TintColor = FSlateColor(FLinearColor::Gray);
	
	BTN_SkillIcon->SetStyle(CurrentStyle);
	bIsUnlock = false;
}