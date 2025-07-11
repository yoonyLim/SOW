// SOWPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "SOWStructTypes.h"
#include "SOWPlayerController.generated.h"

class ATurretPreviewActor;
class UDataTable;
class ASOWCharacterTurretBase;

UCLASS()
class SOW_API ASOWPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable)
    void StartPlacingTurret(FSpellCombination Spells);

    UFUNCTION(BlueprintCallable)
    void ConfirmTurretPlacement();

protected:
    ASOWPlayerController();

    void UpdateTurretPreview();

    UPROPERTY()
    ATurretPreviewActor* PreviewTurret;

    UPROPERTY()
    UDataTable* TurretDataTable;

    FSpellCombination TurretSpells;
    TSubclassOf<ASOWCharacterTurretBase> TurretClass;

    bool bIsPlacingTurret = false;
};