// SOWPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "SOWStructTypes.h"
#include "SOWPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSummonStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSummmonEnd);

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

    UPROPERTY(BlueprintAssignable, Category = "Summon")
    FOnSummonStart SummonStart;

    UPROPERTY(BlueprintAssignable, Category = "Summon")
    FOnSummmonEnd SummonEnd;

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