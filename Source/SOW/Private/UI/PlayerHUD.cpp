// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "AbilitySystem/SOWAttributeSet.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

// Widget
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "UI/SkillSelectWidget.h"
#include "SOWGameInstance.h"
#include "Manager/OneTimeCurrencyManager.h"

void UPlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();

    SkillSelectWidget->SetVisibility(ESlateVisibility::Collapsed);

    USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
    UOneTimeCurrencyManager* OCM = GI->GetOneTimeCurrencyManager();

    OCM->OnOneTimeCurrencyChanged.AddDynamic(this, &UPlayerHUD::ChangeFragment);

    TXT_Fragment->SetText(FText::AsNumber(OCM->GetCurrency(EElementalType::Nature)));

}

void UPlayerHUD::NativeDestruct()
{
    Super::NativeDestruct();

    if (ASC)
    {
        /* Unbind Delegate */
        ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentHealthAttribute()
        ).Remove(HealthChangedHandle);

        ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentManaAttribute()
        ).Remove(ManaChangedHandle);

        ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentStaminaAttribute()
        ).Remove(StaminaChangedHandle);
    }
}

void UPlayerHUD::Init(USOWAbilitySystemComponent* InASC)
{
    checkf(InASC, TEXT("Ability System Component is Invalid in PlayerHUD"));
    BindToASC(InASC);

    if (AttributeSetRef)
    {
        float MaxHealthBase = AttributeSetRef->GetMaxHealthBase();
        float CurrentHealth = AttributeSetRef->GetCurrentHealth();
        SetProgressBar(EStat::HP, MaxHealthBase, CurrentHealth);

        float MaxManaBase = AttributeSetRef->GetMaxManaBase();
        float CurrentMana = AttributeSetRef->GetCurrentMana();
        SetProgressBar(EStat::MP, MaxManaBase, CurrentMana);

        float MaxStaminaBase = AttributeSetRef->GetMaxStaminaBase();
        float CurrentStamina = AttributeSetRef->GetCurrentStamina();
        SetProgressBar(EStat::Stamina, MaxStaminaBase, CurrentStamina);
    }
}

void UPlayerHUD::BindToASC(USOWAbilitySystemComponent* InASC)
{
    ASC = InASC;

    AttributeSetRef = Cast<USOWAttributeSet>(ASC->GetAttributeSet(USOWAttributeSet::StaticClass()));

    if (ASC)
    {
        HealthChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentHealthAttribute()
        ).AddUObject(this, &UPlayerHUD::OnHealthChanged);

        ManaChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentManaAttribute()
        ).AddUObject(this, &UPlayerHUD::OnManaChanged);

        StaminaChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(
            USOWAttributeSet::GetCurrentStaminaAttribute()
        ).AddUObject(this, &UPlayerHUD::OnStaminaChanged);
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
}

void UPlayerHUD::OnManaChanged(const FOnAttributeChangeData& Data)
{
    if (!AttributeSetRef) return;

    float MaxMana = AttributeSetRef->GetMaxManaBase();
    float NewMana = Data.NewValue;

    SetProgressBar(EStat::MP, MaxMana, NewMana);
}

void UPlayerHUD::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
    if (!AttributeSetRef) return;

    float MaxStamina = AttributeSetRef->GetMaxStaminaBase();
    float NewMana = Data.NewValue;

    SetProgressBar(EStat::Stamina, MaxStamina, NewMana);
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

void UPlayerHUD::ChangeFragment(int32 NewCurrency, EElementalType CurrencyType)
{
    UE_LOG(LogTemp, Warning, TEXT("%d"), NewCurrency);

    USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
    UOneTimeCurrencyManager* OCM = GI->GetOneTimeCurrencyManager();

    TXT_Fragment->SetText(FText::AsNumber(OCM->GetCurrency(EElementalType::Nature)));
}