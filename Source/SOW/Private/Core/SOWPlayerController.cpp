// SOWPlayerController.cpp

#include "Core/SOWPlayerController.h"
#include "Characters/Turrets/TurretPreviewActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "SOWStructTypes.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Tile/TileBase.h"

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
    SummonStart.Broadcast();

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
            else
            {
                return;
            }
        }

        PlayerCharacter->ShowInstallationRange(true);
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
    if (bIsPlacingTurret)
    {
        FName TargetRowName = TEXT("SpellComb");

        APawn* ControlledPawn = GetPawn();
        ASOWCharacterPlayer* SOWPlayer = Cast<ASOWCharacterPlayer>(ControlledPawn);

        FHitResult Hit;
        if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit))
        {
            if (bIsPlacingTurret)
            {
                AActor* HitActor = Hit.GetActor();
                if (HitActor)
                {
                    UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
                }

                if (ATileBase* HitTile = Cast<ATileBase>(HitActor))
                {
                    UE_LOG(LogTemp, Log, TEXT("hrerere"));

                    if (HitTile->bCanPlace)
                    {
                        FVector TargetLocation = HitTile->GetActorLocation();
                    
                        bool bCanPlace = FVector::Dist(GetPawn()->GetActorLocation(), TargetLocation) <= 300.f;

                        if (bCanPlace)
                        {
                            USOWBlueprintFunctionLibrary::SpawnTurretWithCircleCount(this, TurretClass, TargetLocation, FRotator::ZeroRotator, 2);

                            PreviewTurret->Destroy();
                            PreviewTurret = nullptr;

                            bIsPlacingTurret = false;
                        }
                        SOWPlayer->bCanMove = true;
                        SOWPlayer->ShowInstallationRange(false);

                        SummonEnd.Broadcast();
                    }
                }
            }
        }
    }
}