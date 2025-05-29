// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SOWGameplayTags.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

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
