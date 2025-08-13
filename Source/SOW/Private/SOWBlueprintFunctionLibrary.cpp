// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SOWGameplayTags.h"
#include "SOWEnumTypes.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AIController.h"
#include "SOWGameInstance.h"
#include "Manager/GlobalCurrencyManager.h"
#include "Manager/OneTimeCurrencyManager.h"

USOWAbilitySystemComponent* USOWBlueprintFunctionLibrary::NativeGetSOWAbilitySystemComponentFromActorInfo(AActor* InActor)
{
    check(InActor);
    USOWAbilitySystemComponent* ASC = CastChecked<USOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));

    return ASC;
}

USOWAbilitySystemComponent* USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(AActor* InActor)
{
    check(InActor);
    return NativeGetSOWAbilitySystemComponentFromActorInfo(InActor);
}

bool USOWBlueprintFunctionLibrary::NativeDoesActorHasTag(AActor* InActor, FGameplayTag InActorTag)
{
    USOWAbilitySystemComponent* ASC = NativeGetSOWAbilitySystemComponentFromActorInfo(InActor);
   
    return  ASC->HasMatchingGameplayTag(InActorTag);
}

bool USOWBlueprintFunctionLibrary::DoesActorHasTag(AActor* InActor, FGameplayTag InActorTag)
{
    return NativeDoesActorHasTag(InActor, InActorTag);
}

bool USOWBlueprintFunctionLibrary::GetMouseWorldLocation(UObject* WorldContextObject, FVector& OutWorldLocation)
{
    if (!WorldContextObject) return false;

    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return false;

    // 0번 인덱스의 플레이어 컨트롤러 가져오기
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (!PC) return false;

    // 마우스 아래 HitResult 가져오기
    FHitResult HitResult;
    if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
    {
        OutWorldLocation = HitResult.ImpactPoint;
        return true;
    }

    return false;
}

bool USOWBlueprintFunctionLibrary::SpawnTurretWithCircleCount(UObject* WorldContextObject, const TSubclassOf<ASOWCharacterTurretBase>& InTurretClass, const FVector& InTargetLoc, const FRotator& InTargetRot, const int32 InCircleCount)
{
    FTransform SpawnTransform;
    SpawnTransform.SetLocation(InTargetLoc);
    SpawnTransform.SetRotation(InTargetRot.Quaternion());

    ASOWCharacterTurretBase* SpawnedTurret = Cast<ASOWCharacterTurretBase>(
        UGameplayStatics::BeginDeferredActorSpawnFromClass(
            WorldContextObject, InTurretClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
        )
    );

    if (SpawnedTurret)
    {
        SpawnedTurret->CircleCount = InCircleCount;

        UGameplayStatics::FinishSpawningActor(SpawnedTurret, SpawnTransform);

        AAIController* AIController = WorldContextObject->GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
        if (AIController && SpawnedTurret)
        {
            AIController->Possess(SpawnedTurret);
        }
        return true;
    }

    return false;
}



bool USOWBlueprintFunctionLibrary::IsTarget(ETurretTargetSelectionPolicy OwnerPolicy, ESOWCharacterType TargetType)
{
    if (OwnerPolicy == ETurretTargetSelectionPolicy::OnEnemy) {
        return TargetType == ESOWCharacterType::Enemy;
    }

    else if (OwnerPolicy == ETurretTargetSelectionPolicy::OnTurret) {
        return TargetType == ESOWCharacterType::Turret;
    }

    else if (OwnerPolicy == ETurretTargetSelectionPolicy::OnPlayer) {
        return TargetType == ESOWCharacterType::Player;
    }

    else {
        return false;
    }
}

AActor* USOWBlueprintFunctionLibrary::GetOwnerActorFromGameplayEffectSpec(const FGameplayEffectSpec& GESpec)
{
    //if (GESpec) return;

    AActor* SourceActor = GESpec.GetContext().GetOriginalInstigator();

    return SourceActor;
}

void USOWBlueprintFunctionLibrary::RequestToGenerateOnTimeCurrency(UObject* WorldContextObject, const FGameplayTag& InTag, const int InCount)
{
    FGameplayEventData Data;
    Data.EventMagnitude = InCount;

    USOWGameInstance* SOWGameInstance = Cast<USOWGameInstance>(WorldContextObject->GetWorld()->GetGameInstance());

    SOWGameInstance->GetGlobalCurrencyManager()->AddCurrency(TranslateElementTagToEnum(InTag), InCount);
    
    UE_LOG(LogTemp, Warning, TEXT("Currency : %s"), *FString::FromInt(SOWGameInstance->GetGlobalCurrencyManager()->GetCurrency(TranslateElementTagToEnum(InTag))));
}

bool USOWBlueprintFunctionLibrary::QueryForCurrencyCountSufficient(UObject* WorldContextObject, const FGameplayTag& InTag, const int InCount)
{
    // Has Enough Currency Count?
    USOWGameInstance* SOWGameInstance = Cast<USOWGameInstance>(WorldContextObject->GetWorld()->GetGameInstance());

    int32 Currency = SOWGameInstance->GetGlobalCurrencyManager()->GetCurrency(TranslateElementTagToEnum(InTag));

    if (Currency >= InCount) {
        RequestToGenerateOnTimeCurrency(WorldContextObject, InTag, -InCount);
        return true;
    }

    return false;
}

EElementalType USOWBlueprintFunctionLibrary::TranslateElementTagToEnum(const FGameplayTag& InTag){

    if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Nature")))) {
        return EElementalType::Nature;
    }

    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Electro")))) {
        return EElementalType::Electro;
    }

    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Wave")))) {
        return EElementalType::Wave;
    }

    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Ice")))) {
        return EElementalType::Ice;
    }

    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Divinity")))) {
        return EElementalType::Divinity;
    }

    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Death")))) {
        return EElementalType::Death;
    }

    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Flame")))) {
        return EElementalType::Flame;
    }
    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Madness")))) {
        return EElementalType::Madness;
    }

    else {
        return EElementalType::Normal;
    }

}