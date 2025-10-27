// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundBase.h"
#include "SOWGameInstance.generated.h"

class USoundManager;
class UUSkillManager;
class UGlobalCurrencyManager;
class UOneTimeCurrencyManager;
class USummonManager;
class UDataTable;

class UTurretSynergyManager;
class ASOWCharacterTurretSpecialBase;


/**
 * 
 */
UCLASS()
class SOW_API USOWGameInstance : public UGameInstance
{
    GENERATED_BODY()
    
public:

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UUSkillManager> SkillManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UGlobalCurrencyManager> GlobalCurrencyManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UOneTimeCurrencyManager> OneTimeCurrencyManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<USummonManager> SummonManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UTurretSynergyManager> TurretSynergyManager;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<USoundManager> SoundManager;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    TObjectPtr<UDataTable> SkillDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Synergy")
    TObjectPtr<UDataTable> SynergyDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Synergy")
    TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioInstance;


public:
    virtual void Init() override;

    void OnStart() override;
    
    UFUNCTION(BlueprintCallable)
    UUSkillManager* GetSkillManager() { return SkillManager; }

    UFUNCTION(BlueprintCallable)
    USummonManager* GetSummonManager() { return SummonManager; }

    UFUNCTION(BlueprintCallable)
    UGlobalCurrencyManager* GetGlobalCurrencyManager() { return GlobalCurrencyManager; }

    UFUNCTION(BlueprintCallable)
    UOneTimeCurrencyManager* GetOneTimeCurrencyManager() { return OneTimeCurrencyManager; }

    UFUNCTION(BlueprintCallable)
    UTurretSynergyManager* GetTurretSynergyManager() { return TurretSynergyManager; }
    UFUNCTION(Exec)
    void SummonTurret(FName TurretName);

    UFUNCTION(Exec)
    void GetResource(float Count);

    void SetWorldTileSize(float InSize);

    UFUNCTION(BlueprintCallable)
    float GetWorldTileSize() const;
    
    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundBase* StartBGM; 


private:

    float WorldTileSize;
};
