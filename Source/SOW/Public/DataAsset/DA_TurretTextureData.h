// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOWEnumTypes.h"
#include "Manager/SummonManager.h"
#include "DA_TurretTextureData.generated.h"

/**
 * 
 */
UCLASS()
class SOW_API UDA_TurretTextureData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "TextrueData")
	TMap<EElementalType ,UTexture2D*> ElementIcons;

	UPROPERTY(EditDefaultsOnly, Category = "TextrueData")
	TMap<ETurretAttackType, UTexture2D*> AtttackTypeIcons;

	UPROPERTY(EditDefaultsOnly, Category = "TextrueData")
	TMap<ERarity, UTexture2D*> RarityFrameTextures;

public:
	UTexture2D* GetElementTexture(EElementalType ElementalType);

	UTexture2D* GetAttackTypeTexture(ETurretAttackType AttackType);

	UTexture2D* GetShopBoxFrameByRarity(ERarity TurretRarity);
};
