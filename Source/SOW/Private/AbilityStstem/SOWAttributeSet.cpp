// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SOWGameplayTags.h"


USOWAttributeSet::USOWAttributeSet()
{
    InitMaxHealth(1.f);
    InitCurrentHealth(1.f);
    InitAttackPowerBase(1.f);
    InitDefensePowerBase(1.f);
    InitDamageTaken(0.f);
    InitDamageOverTime(0.f);
}

void USOWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
   
        // 잊지 말고 부모 구현을 호출하세요.
    Super::PostGameplayEffectExecute(Data);

    // 게임플레이 이펙트가 적용되었다면 디버깅 메시지가 출력됨.
    float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
    SetCurrentHealth(NewCurrentHealth);

    UE_LOG(LogTemp, Warning, TEXT("GameplayEffect applied successfully."), GetCurrentHealth());
    UE_LOG(LogTemp, Warning, TEXT("Current Health : %f"), GetCurrentHealth());

    if (GetCurrentHealth() == 0.f) {
        USOWAbilitySystemComponent* ASC = CastChecked<USOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Data.Target.GetAvatarActor()));

        UE_LOG(LogTemp, Warning, TEXT("Actor has Dead : %f"), GetCurrentHealth());
        ASC->AddLooseGameplayTag(SOWGameplayTags::Shared_Status_Dead);
    }
}

float USOWAttributeSet::GetResistanceForElement(FGameplayTag ElementTag) const
{
    FGameplayTag TargetTag;
    // ElementTag -> Shared.Element.Nature.Major
    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));
    if (ElementTag.MatchesTag(TargetTag)) return GetNatureResistance();

    TargetTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Flame"));
    if (ElementTag.MatchesTag(TargetTag)) return GetFlameResistance();

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

    return 0.f;
}
