// SOWPlayerController.cpp

#include "Core/SOWPlayerController.h"
#include "Characters/Turrets/TurretPreviewActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "SOWStructTypes.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"

#include "SOWBlueprintFunctionLibrary.h"

ASOWPlayerController::ASOWPlayerController()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_Turrets(TEXT("DataTable'/Game/01Blueprints/DataTable/Turrets/DT_TurretSummonData.DT_TurretSummonData'"));
    if (DT_Turrets.Succeeded())
    {
        TurretDataTable = DT_Turrets.Object;
    }
}

void ASOWPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ASOWPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ASOWPlayerController::ConfirmTurretPlacement);
}

void ASOWPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsPlacingTurret)
    {
        UpdateTurretPreview();
    }
}

void ASOWPlayerController::StartPlacingTurret(FSpellCombination Spells)
{
    FName TargetRowName = TEXT("SpellComb");
    TurretSpells = Spells;
    bIsPlacingTurret = true;

    if (!PreviewTurret)
    {
        PreviewTurret = GetWorld()->SpawnActor<ATurretPreviewActor>(ATurretPreviewActor::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("Controller : PreviewTurret Created"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller : PreviewTurret already exists"));
    }

    if (TurretDataTable)
    {
        ASOWCharacterPlayer* PlayerCharacter = Cast<ASOWCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        PlayerCharacter->bCanMove = false;

        for (const auto& Pair : TurretDataTable->GetRowMap())
        {
            const FTurretSummonData* Row = reinterpret_cast<FTurretSummonData*>(Pair.Value);
            if (Row && Row->SpellComb == TurretSpells)
            {
                TurretClass = Row->TurretClass;
                UE_LOG(LogTemp, Log, TEXT("Found matching row: %s"), *Pair.Key.ToString());

                PreviewTurret->SetPreviewActor(Row->Mesh, Row->AttackRange);
            }
        }
    }
}

void ASOWPlayerController::UpdateTurretPreview()
{
    FHitResult Hit;
    if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit) && PreviewTurret)
    {
        FVector HitLocation = Hit.ImpactPoint;
        PreviewTurret->SetActorLocation(HitLocation);

        bool bCanPlace = FVector::Dist(GetPawn()->GetActorLocation(), HitLocation) <= 300.f;
        PreviewTurret->SetCanPlace(bCanPlace);
    }
}

void ASOWPlayerController::ConfirmTurretPlacement()
{
    FName TargetRowName = TEXT("SpellComb");

    UE_LOG(LogTemp, Log, TEXT("Hit Actor: ee"));

    FHitResult Hit;
    if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit))
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Actor: ii"));

        if (bIsPlacingTurret)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor)
            {
                UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
            }

            FVector TargetLocation = Hit.ImpactPoint;

            bool bCanPlace = FVector::Dist(GetPawn()->GetActorLocation(), TargetLocation) <= 200.f;

            if (bCanPlace)
            {
                //GetWorld()->SpawnActor<AActor>(TurretClass, TargetLocation, FRotator::ZeroRotator);
                USOWBlueprintFunctionLibrary::SpawnTurretWithCircleCount(this, TurretClass, TargetLocation, FRotator::ZeroRotator, 2);

                PreviewTurret->Destroy();
                PreviewTurret = nullptr;

                bIsPlacingTurret = false;
            }
            APawn* ControlledPawn = GetPawn();
            if (ASOWCharacterPlayer* SOWPlayer = Cast<ASOWCharacterPlayer>(ControlledPawn))
            {
                SOWPlayer->bCanMove = true;
                SOWPlayer->ShowInstallationRange(false);
            }
        }
        else
        {
            AActor* HitActor = Hit.GetActor();
            UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
        }
    }
}