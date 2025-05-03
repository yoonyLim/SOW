// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SOWGameplayTags.h"

USOWAttributeSet::USOWAttributeSet()
{
    InitCurrentHealth(1.f);

    InitMaxHealth(1.f);
}

void USOWAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
   
        // 잊지 말고 부모 구현을 호출하세요.
    Super::PostGameplayEffectExecute(Data);

    // 게임플레이 이펙트가 적용되었다면 디버깅 메시지가 출력됨.
   /* float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
    SetCurrentHealth(NewCurrentHealth);*/

    UE_LOG(LogTemp, Warning, TEXT("GameplayEffect applied successfully."), GetCurrentHealth());
    UE_LOG(LogTemp, Warning, TEXT("Current Health : %f"), GetCurrentHealth());

    if (GetCurrentHealth() == 0.f) {
        USOWAbilitySystemComponent* ASC = CastChecked<USOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Data.Target.GetAvatarActor()));

        UE_LOG(LogTemp, Warning, TEXT("Actor has Dead : %f"), GetCurrentHealth());
        ASC->AddLooseGameplayTag(SOWGameplayTags::Shared_Status_Dead);
    }
}
