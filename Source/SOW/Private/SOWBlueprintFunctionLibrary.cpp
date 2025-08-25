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
#include "Engine/OverlapResult.h"
#include "Manager/GlobalCurrencyManager.h"
#include "Manager/OneTimeCurrencyManager.h"
#include "Components/BoxComponent.h"

#include "DrawDebugHelpers.h"

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

    float MouseX, MouseY;
    if (!PlayerController->GetMousePosition(MouseX, MouseY)) return SelectedTiles;

    FVector WorldOrigin, WorldDirection;
    if (!PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDirection)) return SelectedTiles;

    FVector TraceStart = WorldOrigin;
    FVector TraceEnd = TraceStart + (WorldDirection * 10000.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    if (!PlayerController->GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Camera, Params)) return SelectedTiles;

    AActor* CenterTile = Hit.GetActor();
    if (!CenterTile) return SelectedTiles;

    FVector CenterLocation = Hit.GetActor()->GetActorLocation();


    // Get Center Position
    /*if (N % 2 == 0) {
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
    }*/

    CenterLocation = MakeCentralTileLocationFromAnyPoint(PlayerController, CenterLocation, TileSelectionType, N, TileSize, true);
   

    switch (TileSelectionType)
    {
    case ETileSelectType::SQUARED:
        return GetTilesAsSquaredFromCenterLocation(PlayerController, CenterLocation, N, TileSize);
    case ETileSelectType::STRAIGHT:
        return GetTilesAsStraightFromCenterLocation(PlayerController, CenterLocation, N, TileSize, true);
    default:
        break;
    }
    return TArray< ATileBase*>();
}

FVector USOWBlueprintFunctionLibrary::MakeCentralTileLocationFromAnyPoint(APlayerController* PlayerController, FVector AnyPoint, const ETileSelectType TileSelectionType, const int32 N, const float TileSize, bool bRot)
{
    FVector CenterLocation = FVector::ZeroVector;

    FVector TraceStart = AnyPoint;
    FVector TraceEnd = AnyPoint + (FVector(0,0,-1) * 10000.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    if (!PlayerController->GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params)) return FVector::ZeroVector;
    CenterLocation = Hit.ImpactPoint;


    switch (TileSelectionType)
    {
        case ETileSelectType::SQUARED:
            if (N % 2 == 0) {
                float DotMax = 0;
                int32 dx[] = { -1,0,1,0 };
                int32 dy[] = { 0,1,0,-1 };
                FVector CenterPos;

                for (int i = 0; i < 4; i++) {
                    FVector Pivot = FVector(TileSize * dx[i], TileSize * dy[i], 0);
                    float Dot = FVector::DotProduct(AnyPoint, Pivot);

                    if (Dot >= DotMax) {
                        DotMax = Dot;
                        CenterPos = CenterLocation + Pivot;
                    }
                }
                CenterLocation = CenterPos;
            }
            else {
                CenterLocation = Hit.GetActor()->GetActorLocation();
            }
            break;

        case ETileSelectType::STRAIGHT:
            if (N % 2 == 0) {
                float DotMax = 0;
                int32 dx[2] ,dy[2];
                FVector CenterPos;

                if (bRot) {
                    dx[0] = 1; dx[1] = -1;
                    dy[0] = 1; dy[1] = -1;
                }
                else {
                    dx[0] = -1; dx[1] = 1;
                    dy[0] = 1; dy[1] = -1;
                }

                for (int i = 0; i < 2; i++) {
                    FVector Pivot = FVector(TileSize * dx[i] / 2.f, TileSize * dy[i] / 2.f , 0);
                    float Dot = FVector::DotProduct(AnyPoint, Pivot);

                    if (Dot >= DotMax) {
                        DotMax = Dot;
                        CenterPos = CenterLocation + Pivot;
                    }
                }
                CenterLocation = CenterPos;
            }
            else {
                CenterLocation = Hit.GetActor()->GetActorLocation();
            }
            break;
    }
    UE_LOG(LogTemp, Warning, TEXT("Central Position : %s, %s, %s"), *FString::SanitizeFloat(CenterLocation.X), *FString::SanitizeFloat(CenterLocation.Y), *FString::SanitizeFloat(CenterLocation.Z));
    return CenterLocation;
}

TArray<ATileBase*> USOWBlueprintFunctionLibrary::GetTilesAsSquaredFromCenterLocation(APlayerController* PlayerController, FVector CenterPosition, const int32 N, const float TileSize)
{
    // Get Tiles Around Center Tile Location.
    // If N is odd, center position must be the center coordinates of the tile.
    // else, it should be the vertex closest to the center coordinates.

    TArray<ATileBase*> SelectedTiles;

    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    FVector RightOffset(0, TileSize, 0.f);
    FVector DownOffset(-TileSize, 0, 0.f);
    FVector OriginOffset = CenterPosition - N/2 * FVector(- TileSize , TileSize, 0);
    if (N % 2 == 0) {
        // 3-1. N이 짝수 일 경우 중심 위치 조정
        OriginOffset += FVector(-TileSize/2, TileSize/2, 0);
    }
    FVector CurrentOffset = OriginOffset;

    for (int32 X = 0; X < N; X++)
    {
        for (int32 Y = 0; Y < N; Y++)
        {

            FVector Start = CurrentOffset + FVector(0, 0, 500.f) + Y * RightOffset;
            FVector End = CurrentOffset + FVector(0, 0, -500.f) + Y * RightOffset;

            FHitResult TileHit;
            if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, End, Start, ECC_Visibility, Params))
            {
                if (TileHit.GetActor() && !SelectedTiles.Contains(TileHit.GetActor()))
                {
                    SelectedTiles.AddUnique(Cast<ATileBase>(TileHit.GetActor()));
                }
            }
        }
        CurrentOffset += DownOffset;
    }
    return SelectedTiles;
}

TArray<ATileBase*> USOWBlueprintFunctionLibrary::GetTilesAsStraightFromCenterLocation(APlayerController* PlayerController, FVector CenterPosition, const int32 N, const float TileSize, bool bRot)
{
    TArray<ATileBase*> SelectedTiles;
    float SideLength = TileSize;

    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    FVector CriticVector = FVector(TileSize, 0, 0) + (bRot ? 1 : -1) * FVector(0, TileSize, 0);

    for (int32 X = 0; X <= N / 2; X++)
    {
        FVector StartRight = CenterPosition + FVector(0, 0, 500.f) + X * CriticVector - (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);
        FVector EndRight = CenterPosition + FVector(0, 0, -500.f) + X * CriticVector - (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);

        FHitResult TileHit;
        if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, EndRight, StartRight, ECC_Visibility, Params))
        {
            if (TileHit.GetActor() && !SelectedTiles.Contains(TileHit.GetActor()))
            {
                SelectedTiles.AddUnique(Cast<ATileBase>(TileHit.GetActor()));
            }
        }

        FVector StartLeft = CenterPosition + FVector(0, 0, 500.f) - X * CriticVector + (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);
        FVector EndLeft = CenterPosition + FVector(0, 0, -500.f) - X * CriticVector + (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);

        if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, EndLeft, StartLeft, ECC_Visibility, Params))
        {
            if (TileHit.GetActor() && !SelectedTiles.Contains(TileHit.GetActor()))
            {
                SelectedTiles.AddUnique(Cast<ATileBase>(TileHit.GetActor()));
            }
        }
    }
    return SelectedTiles;
}

TArray<AActor*> USOWBlueprintFunctionLibrary::GetActorsOnTiles(TArray<ATileBase*> Tiles)
{
    TArray<AActor*> OnTileActors;

    for (ATileBase* tile : Tiles) {

        if (!tile) continue;
        TArray<AActor*> OverlappedActors;

        FVector TileCenter = tile->GetActorLocation(); // 타일 중심
        float HalfExtent = 30.f;                       // 타일 반쪽 크기 (83√2/2 정도)
        float Height = 100.f;                          // Z 높이 범위
        FCollisionShape BoxShape = FCollisionShape::MakeBox(FVector(HalfExtent, HalfExtent, Height));



        TArray<FOverlapResult> Overlaps;

        // 충돌 쿼리 수행
        bool bHasOverlap = tile->GetWorld()->OverlapMultiByChannel(
            Overlaps,
            TileCenter,              // 중심 위치
            FQuat::Identity,         // 회전 (마름모 형태라면 회전 필요)
            ECC_GameTraceChannel1,                // 검사할 채널 (예: Pawn 또는 EnemyPawn 채널)
            BoxShape
        );

        DrawDebugBox(
            tile->GetWorld(),
            TileCenter,                        // 중심 위치
            FVector(HalfExtent, HalfExtent, Height),  // Box 크기
            FQuat::Identity,                   // 회전 (마름모 회전 필요시 수정)
            FColor::Green,                     // 색상
            false,                             // 지속 여부 (true면 무한)
            2.0f,                              // 지속 시간 (초 단위)
            0,                                 // Depth Priority
            2.0f                               // 선 두께
        );

        if (bHasOverlap)
        {
            for (auto& Result : Overlaps)
            {
                AActor* HitActor = Result.GetActor();
                if (HitActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Detected Actor on Tile %s : %s"),
                        *tile->GetActorNameOrLabel(),
                        *HitActor->GetActorNameOrLabel());
                }

                UE_LOG(LogTemp, Warning, TEXT("Detected Actor : %s"), *HitActor->GetActorNameOrLabel());
                OnTileActors.AddUnique(HitActor);
            }
        }

        //UBoxComponent* OverlapBox = NewObject<UBoxComponent>(tile);
        //UE_LOG(LogTemp, Warning, TEXT("Component for %s : %s"), *tile ->GetActorNameOrLabel(), *OverlapBox->GetName());
        //// 크기 설정 (타일 변의 길이가 83*√2 라면 HalfExtent는 절반 값)
        //float ExtentSize = 50.f;
        //OverlapBox->SetBoxExtent(FVector(ExtentSize, ExtentSize, 20)); // Z는 임의 높이
        //OverlapBox->SetRelativeLocation(FVector(0, 0, 300.f)); // Z는 임의 높이

        //OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);        // 물리 충돌(Physics)은 안하고, 오버랩/트레이스만 감지
        //OverlapBox->SetCollisionObjectType(ECC_WorldDynamic);                // 객체 타입 설정 (보통 WorldDynamic)
        //OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);           // 모든 채널 무시

        //OverlapBox->SetCollisionProfileName(TEXT("EnemyPawn"));
        //OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);    // Pawn에 대해서만 오버랩 감지
        ////OverlapBox->SetCollisionResponseToChannel(ECC_EnemyPawn, ECR_Overlap);    // Pawn에 대해서만 오버랩 감지

        //OverlapBox->bHiddenInGame = false;
        ////OverlapBox->SetRelativeRotation(FRotator(0.f, 45.f, 0.f));

        //// 활성화
        //OverlapBox->RegisterComponent();
        //OverlapBox->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        //// Overlap된 액터 가져오기
        //
        //OverlapBox->GetOverlappingActors(OverlappedActors, ASOWCharacter::StaticClass());

        //OverlapBox->DestroyComponent();

        //for (AActor* Actor : OverlappedActors) {
        //    OnTileActors.AddUnique(Actor);
        //}
    }
    for (AActor* Actor : OnTileActors) {
        UE_LOG(LogTemp, Warning, TEXT("Final Actor : %s"), *Actor->GetActorNameOrLabel());
    }
    return OnTileActors;
}
