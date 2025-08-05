// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SOWGameplayTags.h"

#include "Components/SOWCharacterUIComponent.h"
#include "Interface/SOWCharacterUIInterface.h"


USOWAttributeSet::USOWAttributeSet()

{
    InitWalkSpeed(400.f);
    InitMaxHealthBase(100.f);
    InitCurrentHealth(100.f);
    InitAttackPowerBase(1.f);
    InitDefensePowerBase(1.f);
    InitDamageOverTime(0.f);
    InitDetectionRange(50.f);
    InitAttackSpeedBase(1.f);
    InitMaxManaBase(200.f);
    InitCurrentMana(200.f);
    InitMaxStaminaBase(100.f);
    InitCurrentStamina(GetMaxStaminaBase());
}

void USOWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

    Super::PostGameplayEffectExecute(Data);

    if (!CachedCharacterUIInterface.IsValid()) {
        CachedCharacterUIInterface = TWeakInterfacePtr<ISOWCharacterUIInterface>(Data.Target.GetAvatarActor());
    }
    checkf(CachedCharacterUIInterface.IsValid(), TEXT("CachedCharacterUIInterface has not implemented for %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

    USOWCharacterUIComponent* CharacterUIComponent = CachedCharacterUIInterface->GetCharacterUIComponent();



    USOWAbilitySystemComponent* ASC = CastChecked<USOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Data.Target.GetAvatarActor()));
    if (!ASC) return;

    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
        float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealthBase());
        SetCurrentHealth(NewCurrentHealth);

        UE_LOG(LogTemp, Warning, TEXT("Actor has Current Health : %f"), GetCurrentHealth());

        if (CharacterUIComponent && CharacterUIComponent->OnCurrentHealthChanged.IsBound())
        {
            CharacterUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealthBase());
        }
    }
    else if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute()) {
        float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStaminaBase());
        SetCurrentStamina(NewCurrentStamina);

        UE_LOG(LogTemp, Warning, TEXT("Actor has Current Stamina : %f"), GetCurrentStamina());
    }
    else if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute()) {
        float NewCurrentMana = FMath::Clamp(GetCurrentMana(), 0.f, GetMaxManaBase());
        SetCurrentMana(NewCurrentMana);

        UE_LOG(LogTemp, Warning, TEXT("Actor has Current Mana : %f"), GetCurrentMana());
    }



    if (GetCurrentHealth() == 0.f) {
        
        if (!ASC->HasMatchingGameplayTag(SOWGameplayTags::Shared_Status_Dead)) {
            UE_LOG(LogTemp, Warning, TEXT("Actor has Dead : %f"), GetCurrentHealth());
            ASC->AddLooseGameplayTag(SOWGameplayTags::Shared_Status_Dead);
        }
    }
}

float USOWAttributeSet::GetResistanceForElementWithElementTag(FGameplayTag ElementTag) const
{
    if (!ElementTag.IsValid()) return 0.f;
    
    FGameplayTag TargetTag;

    // ElementTag -> Shared.Element.Nature.Major
    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));
    if (ElementTag.MatchesTag(TargetTag)) return GetNatureResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Electro"));
    if (ElementTag.MatchesTag(TargetTag)) return GetElectroResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Ice"));
    if (ElementTag.MatchesTag(TargetTag)) return GetIceResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Wave"));
    if (ElementTag.MatchesTag(TargetTag)) return GetWaveResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Death"));
    if (ElementTag.MatchesTag(TargetTag)) return GetDeathResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Divinity"));
    if (ElementTag.MatchesTag(TargetTag)) return GetDivinityResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Madness"));
    if (ElementTag.MatchesTag(TargetTag)) return GetMadnessResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Flame"));
    if (ElementTag.MatchesTag(TargetTag)) return GetFlameResistance();

    return 0.f;
}
