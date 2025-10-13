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
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameModes/WaveGameMode.h"
#include "Blueprint/WidgetLayoutLibrary.h"
//#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/SOWWidgetBase.h"
#include "Slate/SObjectWidget.h"

#include "Framework/Application/SlateApplication.h"

#include "DrawDebugHelpers.h"

// TileSize 를 인게임 타일 사이즈를 가져오는 Getter로 모두 전환하면 끝임.

USOWAbilitySystemComponent* USOWBlueprintFunctionLibrary::NativeGetSOWAbilitySystemComponentFromActorInfo(AActor* InActor)
{
    if (!IsValid(InActor))
    {
        //UE_LOG(LogTemp, Warning, TEXT("NativeGetSOWAbilitySystemComponentFromActorInfo: Invalid Actor"));
        return nullptr;
    }

    USOWAbilitySystemComponent* ASC =
        Cast<USOWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));

    return ASC;
}

USOWAbilitySystemComponent* USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(AActor* InActor)
{
    if (!IsValid(InActor))
    {
        //UE_LOG(LogTemp, Warning, TEXT("GetSOWAbilitySystemComponentFromActorInfo: Invalid Actor"));
        return nullptr;
    }
    return NativeGetSOWAbilitySystemComponentFromActorInfo(InActor);
}

bool USOWBlueprintFunctionLibrary::NativeDoesActorHasTag(AActor* InActor, FGameplayTag InActorTag)
{
    if (!IsValid(InActor))
    {
       // UE_LOG(LogTemp, Warning, TEXT("NativeDoesActorHasTag: Invalid Actor"));
        return false;
    }

    USOWAbilitySystemComponent* ASC = NativeGetSOWAbilitySystemComponentFromActorInfo(InActor);
    if (!ASC)
    {
       // UE_LOG(LogTemp, Warning, TEXT("NativeDoesActorHasTag: No ASC for %s"), *InActor->GetName());
        return false;
    }

    return ASC->HasMatchingGameplayTag(InActorTag);
}

bool USOWBlueprintFunctionLibrary::DoesActorHasTag(AActor* InActor, FGameplayTag InActorTag)
{
    // ✅ 반드시 IsValid 사용해야 함
    if (!IsValid(InActor))
    {
        //UE_LOG(LogTemp, Warning, TEXT("DoesActorHasTag: Invalid Actor pointer (possibly destroyed)."));
        return false;
    }

    // ✅ UClass 접근 전에도 반드시 유효성 체크
    UClass* ActorClass = InActor->GetClass();
    if (ActorClass == nullptr)
    {
       // UE_LOG(LogTemp, Warning, TEXT("DoesActorHasTag: Invalid ActorClass for %s"), *InActor->GetName());
        return false;
    }

    // ✅ 안전한 방식으로 인터페이스 확인
    if (!ActorClass->ImplementsInterface(USOWCharacterTypeInterface::StaticClass()))
    {
        //UE_LOG(LogTemp, VeryVerbose, TEXT("%s does not implement SOWCharacterTypeInterface"), *InActor->GetName());
        return false;
    }

    return NativeDoesActorHasTag(InActor, InActorTag);
}

///////////////////////////////

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

    if (IsValid(SpawnedTurret))
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

    //SOWGameInstance->GetOneTimeCurrencyManager()->AddCurrency(TranslateElementTagToEnum(InTag), InCount);
    SOWGameInstance->GetResource(InCount);
    
    UE_LOG(LogTemp, Warning, TEXT("Currency : %s"), *FString::FromInt(SOWGameInstance->GetOneTimeCurrencyManager()->GetCurrency(TranslateElementTagToEnum(InTag))));
}

bool USOWBlueprintFunctionLibrary::QueryForCurrencyCountSufficient(UObject* WorldContextObject, const FGameplayTag& InTag, const int InCount)
{
    // Has Enough Currency Count?
    USOWGameInstance* SOWGameInstance = Cast<USOWGameInstance>(WorldContextObject->GetWorld()->GetGameInstance());

    AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(WorldContextObject->GetWorld()));

    int32 Currency = GM->GetCurrency();

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

EElementalType USOWBlueprintFunctionLibrary::FindTurretElementAsEnum(ASOWCharacterTurretBase* InTurret)
{
    if (!IsValid(InTurret)) return EElementalType::Max;

    return TranslateElementTagToEnum(FindTurretElementAsTag(InTurret));
}

FGameplayTag USOWBlueprintFunctionLibrary::FindTurretElementAsTag(ASOWCharacterTurretBase* InTurret)
{
    if (!IsValid(InTurret))
        return FGameplayTag();

    // Turret이 가진 ASC 가져오기
    USOWAbilitySystemComponent* ASC = NativeGetSOWAbilitySystemComponentFromActorInfo(InTurret);
    if (!IsValid(ASC))
        return FGameplayTag();

    // ASC에서 태그 컨테이너 복사
    const FGameplayTagContainer& AllTags = ASC->GetOwnedGameplayTags();

    // 기준 태그
    const FGameplayTag ElementParentTag = FGameplayTag::RequestGameplayTag(TEXT("Shared.Element"));

    // Shared.Element 기반 태그 탐색
    for (const FGameplayTag& Tag : AllTags)
    {
        if (Tag.MatchesTag(ElementParentTag))
        {
            return Tag;
        }
    }

    return FGameplayTag(); // 못 찾으면 빈 값 반환
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
    if (!IsValid(CenterTile)) return SelectedTiles;

    FVector CenterLocation = Hit.GetActor()->GetActorLocation();

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
    USOWGameInstance* GI = Cast<USOWGameInstance>(PlayerController->GetWorld()->GetGameInstance());
    float WorldTileSize = GI->GetWorldTileSize();

    FVector CenterLocation = FVector::ZeroVector;

    FVector TraceStart = AnyPoint;
    FVector TraceEnd = AnyPoint + (FVector(0,0,-1) * 10000.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    if (!PlayerController->GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel1, Params)) return FVector::ZeroVector;
    if(!IsValid(Hit.GetActor())) return FVector::ZeroVector;

    CenterLocation = Hit.GetActor()->GetActorLocation();
    FVector CriticVector = AnyPoint - CenterLocation;

    switch (TileSelectionType)
    {
        case ETileSelectType::SQUARED:
            if (N % 2 == 0) {
                float DotMax = 0;
                int32 dx[] = { -1,1,-1,1 };
                int32 dy[] = { 1,1,-1,-1 };
                FVector CenterPos;

                for (int i = 0; i < 4; i++) {
                    FVector Pivot = FVector(WorldTileSize / 2 * dx[i], WorldTileSize / 2 * dy[i], 0);
                    //FVector Pivot = FVector(TileSize/2 * dx[i], TileSize/2 * dy[i], 0);
                    float Dot = FVector::DotProduct(CriticVector, Pivot);

                    if (Dot >= DotMax) {
                        DotMax = Dot;
                        CenterPos = CenterLocation + Pivot;
                    }
                }
                CenterLocation = CenterPos;
            }
            break;

        case ETileSelectType::STRAIGHT:
            // need to fix
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
                    FVector Pivot = FVector(WorldTileSize * dx[i] / 2.f, WorldTileSize * dy[i] / 2.f, 0);
                   // FVector Pivot = FVector(TileSize * dx[i] / 2.f, TileSize * dy[i] / 2.f , 0);
                    float Dot = FVector::DotProduct(AnyPoint, Pivot);

                    if (Dot >= DotMax) {
                        DotMax = Dot;
                        CenterPos = CenterLocation + Pivot;
                    }
                }
                CenterLocation = CenterPos;
            }
            break;
    }
    UE_LOG(LogTemp, Warning, TEXT("Central Position : %s, %s, %s"), *FString::SanitizeFloat(CenterLocation.X), *FString::SanitizeFloat(CenterLocation.Y), *FString::SanitizeFloat(CenterLocation.Z));
    return CenterLocation;
}

TArray<ATileBase*> USOWBlueprintFunctionLibrary::GetTilesAsSquaredFromCenterLocation(APlayerController* PlayerController, FVector CenterPosition, const int32 N, const float TileSize)
{
    USOWGameInstance* GI = Cast<USOWGameInstance>(PlayerController->GetWorld()->GetGameInstance());
    float WorldTileSize = GI->GetWorldTileSize();
    // Get Tiles Around Center Tile Location.
    // If N is odd, center position must be the center coordinates of the tile.
    // else, it should be the vertex closest to the center coordinates.

    TArray<ATileBase*> SelectedTiles;

    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    FVector RightOffset(WorldTileSize, 0.f, 0.f);
    FVector DownOffset(0, WorldTileSize, 0.f);
    FVector OriginOffset = CenterPosition - N / 2 * FVector(WorldTileSize, WorldTileSize, 0);

   /* FVector RightOffset(TileSize, 0.f, 0.f);
    FVector DownOffset(0, TileSize, 0.f);
    FVector OriginOffset = CenterPosition - N/2 * FVector(TileSize , TileSize, 0);*/
    if (N % 2 == 0) {
        // 3-1. N이 짝수 일 경우 중심 위치 조정
        OriginOffset += FVector(WorldTileSize / 2, WorldTileSize / 2, 0);
        //OriginOffset += FVector(TileSize/2, TileSize/2, 0);
    }
    FVector CurrentOffset = OriginOffset;

    for (int32 X = 0; X < N; X++)
    {
        for (int32 Y = 0; Y < N; Y++)
        {

            FVector Start = CurrentOffset + X * DownOffset + FVector(0, 0, 500.f) + Y * RightOffset;
            FVector End = CurrentOffset + X * DownOffset + FVector(0, 0, -500.f) + Y * RightOffset;

            FHitResult TileHit;
            if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, End, Start, ECC_GameTraceChannel1, Params))
            {
                if (TileHit.GetActor() && !SelectedTiles.Contains(TileHit.GetActor()))
                {
                    SelectedTiles.AddUnique(Cast<ATileBase>(TileHit.GetActor()));
                }
            }
        }
    }
    return SelectedTiles;
}

TArray<ATileBase*> USOWBlueprintFunctionLibrary::GetTilesAsStraightFromCenterLocation(APlayerController* PlayerController, FVector CenterPosition, const int32 N, const float TileSize, bool bRot)
{
    USOWGameInstance* GI = Cast<USOWGameInstance>(PlayerController->GetWorld()->GetGameInstance());
    float WorldTileSize = GI->GetWorldTileSize();

    TArray<ATileBase*> SelectedTiles;

    float SideLength = WorldTileSize;
   // float SideLength = TileSize;

    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = false;

    FVector CriticVector = FVector(WorldTileSize, 0, 0) + (bRot ? 1 : -1) * FVector(0, WorldTileSize, 0);
    //FVector CriticVector = FVector(TileSize, 0, 0) + (bRot ? 1 : -1) * FVector(0, TileSize, 0);

    for (int32 X = 0; X <= N / 2; X++)
    {
        FVector StartRight = CenterPosition + FVector(0, 0, 500.f) + X * CriticVector - (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);
        FVector EndRight = CenterPosition + FVector(0, 0, -500.f) + X * CriticVector - (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);

        FHitResult TileHit;
        if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, EndRight, StartRight, ECC_GameTraceChannel1, Params))
        {
            if (TileHit.GetActor() && !SelectedTiles.Contains(TileHit.GetActor()))
            {
                SelectedTiles.AddUnique(Cast<ATileBase>(TileHit.GetActor()));
            }
        }

        FVector StartLeft = CenterPosition + FVector(0, 0, 500.f) - X * CriticVector + (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);
        FVector EndLeft = CenterPosition + FVector(0, 0, -500.f) - X * CriticVector + (N % 2 == 0 ? CriticVector/2 : FVector::ZeroVector);

        if (PlayerController->GetWorld()->LineTraceSingleByChannel(TileHit, EndLeft, StartLeft, ECC_GameTraceChannel1, Params))
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

        if (!IsValid(tile)) continue;
        TArray<AActor*> OverlappedActors;

        FVector TileCenter = tile->GetActorLocation(); 
        float HalfExtent = 25.f;                       
        float Height = 200.f;                          
        FCollisionShape BoxShape = FCollisionShape::MakeBox(FVector(HalfExtent, HalfExtent, Height));



        TArray<FOverlapResult> Overlaps;

        // 충돌 쿼리 수행
        bool bHasOverlap = tile->GetWorld()->OverlapMultiByChannel(
            Overlaps,
            TileCenter,              
            FQuat::Identity,         
            ECC_GameTraceChannel1,                
            BoxShape
        );

        if (bHasOverlap)
        {
            for (auto& Result : Overlaps)
            {
                AActor* HitActor = Result.GetActor();
                if (IsValid(HitActor))
                {
                    OnTileActors.AddUnique(HitActor);
                }

                
            }
        }
    }
    return OnTileActors;
}

TArray<AActor*> USOWBlueprintFunctionLibrary::GetTurretsOnTiles(TArray<ATileBase*> Tiles)
{
    TArray<AActor*> OnTileActors;

    for (ATileBase* tile : Tiles) {

        if (!IsValid(tile)) continue;
        TArray<AActor*> OverlappedActors;

        FVector TileCenter = tile->GetActorLocation();
        float HalfExtent = 25.f;
        float Height = 100.f;
        FCollisionShape BoxShape = FCollisionShape::MakeBox(FVector(HalfExtent, HalfExtent, Height));



        TArray<FOverlapResult> Overlaps;

        // 충돌 쿼리 수행
        bool bHasOverlap = tile->GetWorld()->OverlapMultiByChannel(
            Overlaps,
            TileCenter,
            FQuat::Identity,
            ECC_GameTraceChannel3,
            BoxShape
        );

        if (bHasOverlap)
        {
            for (auto& Result : Overlaps)
            {
                AActor* HitActor = Result.GetActor();
                if (IsValid(HitActor))
                {
                    OnTileActors.AddUnique(HitActor);
                }


            }
        }
    }
    return OnTileActors;
}

bool USOWBlueprintFunctionLibrary::IsMouseOverUI(APlayerController* PC, const TSubclassOf<USOWWidgetBase>& TargetWidget)
{
    if (!FSlateApplication::IsInitialized())
        return false;

    FSlateApplication& SlateApp = FSlateApplication::Get();
    FVector2D MousePos = SlateApp.GetCursorPos();

    FWidgetPath WidgetPath = SlateApp.LocateWindowUnderMouse(MousePos, SlateApp.GetInteractiveTopLevelWindows());
    if (!WidgetPath.IsValid())
        return false;

    const FArrangedChildren& ArrangedWidgets = WidgetPath.Widgets;

    for (const FArrangedWidget& ArrangedWidget : ArrangedWidgets.GetInternalArray())
    {
        TSharedPtr<SWidget> SlateWidget = ArrangedWidget.Widget;
        if (!SlateWidget.IsValid())
            continue;

        // 🔹 핵심 부분: SObjectWidget을 통해 UUserWidget 찾기
        if (TSharedPtr<SObjectWidget> ObjectWidget = StaticCastSharedPtr<SObjectWidget>(SlateWidget))
        {
            if (!ObjectWidget.IsValid()) continue;
            UUserWidget* UserWidget = ObjectWidget->GetWidgetObject();

            if (IsValid(UserWidget))
            {
                if (UserWidget->IsA(TargetWidget))
                {
                    return true; // 특정 위젯 클래스 위에 마우스가 존재함
                }
            }
        }
    }

    return false;
}



