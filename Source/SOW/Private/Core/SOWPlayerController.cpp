// SOWPlayerController.cpp

#include "Core/SOWPlayerController.h"
#include "Characters/Turrets/TurretPreviewActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "SOWStructTypes.h"

ASOWPlayerController::ASOWPlayerController()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_Turrets(TEXT("DataTable'/Game/01Blueprints/DataTable/Turrets/DT_TurretData.DT_TurretData'"));
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
    InputComponent->BindAction("ConfirmTurret", IE_Pressed, this, &ASOWPlayerController::ConfirmTurretPlacement);
}

void ASOWPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsPlacingTurret)
    {
        UpdateTurretPreview();
    }
}

void ASOWPlayerController::StartPlacingTurret(FName TurretRowName)
{
    PendingTurretName = TurretRowName;
    bIsPlacingTurret = true;

    const FTurretData* Row = TurretDataTable->FindRow<FTurretData>(PendingTurretName, TEXT(""));
    if (!Row || !Row->Mesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Controller : Invalid turret data for %s"), *TurretRowName.ToString());
        return;
    }

    if (!PreviewTurret)
    {
        PreviewTurret = GetWorld()->SpawnActor<ATurretPreviewActor>(ATurretPreviewActor::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("Controller : PreviewTurret Created"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller : PreviewTurret already exists"));
    }

    PreviewTurret->SetSkeletalMesh(Row->Mesh);
}

void ASOWPlayerController::UpdateTurretPreview()
{
    FHitResult Hit;
    if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit) && PreviewTurret)
    {
        FVector HitLocation = Hit.ImpactPoint;
        PreviewTurret->SetActorLocation(HitLocation);

        bool bCanPlace = FVector::Dist(GetPawn()->GetActorLocation(), HitLocation) <= 200.f;
        PreviewTurret->SetCanPlace(bCanPlace);
    }
}

void ASOWPlayerController::ConfirmTurretPlacement()
{
    if (!bIsPlacingTurret || !PreviewTurret || PendingTurretName == NAME_None) return;


    FHitResult Hit;
    if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, Hit))
    {
        FVector TargetLocation = Hit.ImpactPoint;

        bool bCanPlace = FVector::Dist(GetPawn()->GetActorLocation(), TargetLocation) <= 200.f;

        if (bCanPlace)
        {
            const FTurretData* Row = TurretDataTable->FindRow<FTurretData>(PendingTurretName, TEXT(""));

            if (Row && Row->TurretClass)
            {
                GetWorld()->SpawnActor<AActor>(Row->TurretClass, TargetLocation, FRotator::ZeroRotator);

                PreviewTurret->Destroy();
                PreviewTurret = nullptr;

                bIsPlacingTurret = false;
                PendingTurretName = NAME_None;
            }
        }
    }
}