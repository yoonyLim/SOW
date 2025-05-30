// SOWPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "SOWPlayerController.generated.h"

class ATurretPreviewActor;
class UDataTable;

UCLASS()
class SOW_API ASOWPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable)
    void StartPlacingTurret(FName TurretRowName);

    void ConfirmTurretPlacement();

protected:
    ASOWPlayerController();

    void UpdateTurretPreview();

    UPROPERTY()
    ATurretPreviewActor* PreviewTurret;

    UPROPERTY()
    UDataTable* TurretDataTable;

    FName PendingTurretName;

    bool bIsPlacingTurret = false;
};