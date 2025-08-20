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
#include "Tile/TileBase.h"
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

    SOWGameInstance->GetOneTimeCurrencyManager()->AddCurrency(TranslateElementTagToEnum(InTag), InCount);
    
    UE_LOG(LogTemp, Warning, TEXT("Currency : %s"), *FString::FromInt(SOWGameInstance->GetOneTimeCurrencyManager()->GetCurrency(TranslateElementTagToEnum(InTag))));
}

bool USOWBlueprintFunctionLibrary::QueryForCurrencyCountSufficient(UObject* WorldContextObject, const FGameplayTag& InTag, const int InCount)
{
    // Has Enough Currency Count?
    USOWGameInstance* SOWGameInstance = Cast<USOWGameInstance>(WorldContextObject->GetWorld()->GetGameInstance());

    int32 Currency = SOWGameInstance->GetOneTimeCurrencyManager()->GetCurrency(TranslateElementTagToEnum(InTag));

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

TArray<ATileBase*> USOWBlueprintFunctionLibrary::GetTilesAroundMouse(APlayerController* PlayerController, const ETileSelectType TileSelectionType, const int32 N, const float TileSize = 83.f)
{
    TArray<ATileBase*> SelectedTiles;

    if (!PlayerController || N <= 0) return SelectedTiles;

    // 1. 마우스 위치의 타일 인식
    float MouseX, MouseY;
    if (!PlayerController->GetMousePosition(MouseX, MouseY)) return SelectedTiles;

    FVector WorldOrigin, WorldDirection;
    if (!PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDirection))
        return SelectedTiles;

    FVector TraceStart = WorldOrigin;
    FVector TraceEnd = TraceStart + (WorldDirection * 10000.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    if (!PlayerController->GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Camera, Params))
        return SelectedTiles;

    AActor* CenterTile = Hit.GetActor();
    if (!CenterTile) return SelectedTiles;

    // 2. 중심 타일의 중심 위치 확인
    FVector CenterLocation = Hit.GetActor()->GetActorLocation();
    if (N % 2 == 0) {
        // 2-1. N이 짝수 일 경우 중심 위치 조정
        float DotMax = 0;
        int32 dx[] = {-1,0,1,0};
        int32 dy[] = { 0,1,0,-1 };
        FVector CenterPos;

        FVector MouseVector = (Hit.ImpactPoint - CenterLocation);
        for (int i = 0; i < 4; i++) {
            FVector Pivot = FVector(TileSize * dx[i], TileSize * dy[i], 0);
            float Dot = FVector::DotProduct(MouseVector, Pivot);

            if (Dot >= DotMax) {
                DotMax = Dot;
                CenterPos = CenterLocation + Pivot;
            }
        }
       
        CenterLocation = CenterPos;
    }


    // 3. 중심 위치에서 n x n 정사각형 영역 트레이싱
    float SideLength = TileSize;

    FVector UpperOffset(SideLength, SideLength, 0.f);
    FVector DownOffset(-SideLength, SideLength, 0.f);
    FVector OriginOffset = CenterLocation - (N / 2) * FVector(0, TileSize * 2, 0);
    if (N % 2 == 0) {
        // 3-1. N이 짝수 일 경우 중심 위치 조정
        OriginOffset += FVector(0, TileSize, 0);
    }
    FVector CurrentOffSet = OriginOffset;
    


    UE_LOG(LogTemp, Warning, TEXT("Center Pos : %s, %s, %s"), * FString::SanitizeFloat(CenterLocation.X), *FString::SanitizeFloat(CenterLocation.Y), *FString::SanitizeFloat(CenterLocation.Z));
    UE_LOG(LogTemp, Warning, TEXT("Origin Pos : %s, %s, %s"), * FString::SanitizeFloat(OriginOffset.X), *FString::SanitizeFloat(OriginOffset.Y), *FString::SanitizeFloat(OriginOffset.Z));

    for (int32 X = 0; X < N; X++)
    {
        
        UE_LOG(LogTemp, Warning, TEXT("Current Pos : %s, %s, %s"), *FString::SanitizeFloat(CurrentOffSet.X), *FString::SanitizeFloat(CurrentOffSet.Y), *FString::SanitizeFloat(CurrentOffSet.Z));
        for (int32 Y = 0; Y < N; Y++)
        {
           
            FVector Start = CurrentOffSet + FVector(0, 0, 500.f) + Y * UpperOffset;
            FVector End = CurrentOffSet + FVector(0, 0, -500.f) + Y * UpperOffset;

            FHitResult TileHit;
            if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, Start, End, ECC_Visibility, Params))
            {
                if (TileHit.GetActor() && !SelectedTiles.Contains(TileHit.GetActor()))
                {
                    SelectedTiles.AddUnique(Cast<ATileBase>(TileHit.GetActor()));
                }
            }
        }
        CurrentOffSet += DownOffset;
    }

    // 5. 타일 배열 반환
    return SelectedTiles;
}