// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWEnumTypes.h"
#include "SynergyInfoWidget.generated.h"


class UImage;
class ASynergyUpdateAnnouncer;

/**
 * 
 */
UCLASS()
class SOW_API USynergyInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EElementalType Synergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ActivateSynergyCountIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DeactivateSynergyCountIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UImage*> TurretPortraits;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TMap<FName, FLinearColor> TurretPortraitsColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> DetailContainers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> SynergyCountIcons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ASynergyUpdateAnnouncer* SUA;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateSynergyInfoWidget(EElementalType ElementType, int SynergyCount, const TArray<FName>& SynergyComposer);

	UFUNCTION(BlueprintCallable)
	void UpdateDetailContainer();

	UFUNCTION(BlueprintCallable)
	void UpdateSynergyCountIcon();

	//void UpdateTurretPortrait(const TArray<FName>& SynergyComposer);

protected:
	void NativeConstruct();
};
