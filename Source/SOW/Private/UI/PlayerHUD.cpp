// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "AbilitySystem/SOWAttributeSet.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

// Widget
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void UPlayerHUD::NativeConstruct()
{
}

void UPlayerHUD::Init(USOWAbilitySystemComponent* InASC)
{
    checkf(InASC, TEXT("Ability System Component is Invalid in PlayerHUD"));
    BindToASC(InASC);

    if (AttributeSetRef)
    {
        float MaxHealth = AttributeSetRef->GetMaxHealthBase();
        float CurrentHealth = AttributeSetRef->GetCurrentHealth();
        SetProgressBar(EStat::HP, MaxHealth, CurrentHealth);
    }
    //float MaxMana = AttributeSetRef->GetMaxMana();
    //float CurrentMana = AttributeSetRef->GetCurrentMana();
    //SetProgressBar(EPlayerStat::MP, MaxHealth, CurrentHealth);

    //float MaxHealth = AttributeSetRef->GetMaxHealth();
    //float CurrentHealth = AttributeSetRef->GetCurrentHealth();
    //SetProgressBar(EPlayerStat::Stamina, MaxHealth, CurrentHealth);
}

void UPlayerHUD::BindToASC(USOWAbilitySystemComponent* InASC)
{
    ASC = InASC;

    AttributeSetRef = Cast<USOWAttributeSet>(ASC->GetAttributeSet(USOWAttributeSet::StaticClass()));

    if (ASC)
    {
        ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentHealthAttribute()
        ).AddUObject(this, &UPlayerHUD::OnHealthChanged);
    }

    if (!AttributeSetRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("HUD : Fail to bind AttributeSet"));
    }
}

void UPlayerHUD::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    if (!AttributeSetRef) return;

    float MaxHealth = AttributeSetRef->GetMaxHealthBase();
    float NewHealth = Data.NewValue;

    SetProgressBar(EStat::HP, MaxHealth, NewHealth);
    // 여기에 HealthBar->SetPercent(NewHealth / MaxHealth); 등을 작성
}

void UPlayerHUD::SetProgressBar(EStat ChangedStat, float Max, float Current)
{
    switch (ChangedStat)
    {
    case EStat::HP:
        HP_Bar->SetPercent(Current / Max);
        break;

    case EStat::MP:
        MP_Bar->SetPercent(Current / Max);
        break;

    case EStat::Stamina:
        Stamina_Bar->SetPercent(Current / Max);
        break;
    }
    return;
}