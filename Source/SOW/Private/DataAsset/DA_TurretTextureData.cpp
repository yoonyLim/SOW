// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DA_TurretTextureData.h"

UTexture2D* UDA_TurretTextureData::GetElementTexture(EElementalType ElementalType)
{
	//return ElementIcons[ElementalType];
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("UDA_TurretTextureData::GetAttackTypeTexture called on nullptr"));
        return nullptr;
    }

    if (!ElementIcons.Contains(ElementalType))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid AttackType %d in %s"), (int32)ElementalType, *GetName());
        return nullptr;
    }

    UTexture2D* Texture = ElementIcons[ElementalType];

    if (!Texture)
    {
        UE_LOG(LogTemp, Error, TEXT("Texture not set for AttackType %d in %s"), (int32)ElementalType, *GetName());
    }

    return Texture;
}

UTexture2D* UDA_TurretTextureData::GetAttackTypeTexture(ETurretAttackType AttackType)
{
	//return AtttackTypeIcons[AttackType];
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("UDA_TurretTextureData::GetAttackTypeTexture called on nullptr"));
        return nullptr;
    }

    if (!AtttackTypeIcons.Contains(AttackType))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid AttackType %d in %s"), (int32)AttackType, *GetName());
        return nullptr;
    }

    UTexture2D* Texture = AtttackTypeIcons[AttackType];

    if (!Texture)
    {
        UE_LOG(LogTemp, Error, TEXT("Texture not set for AttackType %d in %s"), (int32)AttackType, *GetName());
    }

    return Texture;

}

UTexture2D* UDA_TurretTextureData::GetShopBoxFrameByRarity(ERarity TurretRarity)
{
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("UDA_TurretTextureData::GetShopBoxFrameByRarity called on nullptr"));
        return nullptr;
    }

    if (!RarityFrameTextures.Contains(TurretRarity))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid AttackType %d in %s"), (int32)TurretRarity, *GetName());
        return nullptr;
    }

    UTexture2D* Texture = RarityFrameTextures[TurretRarity];

    if (!Texture)
    {
        UE_LOG(LogTemp, Error, TEXT("Texture not set for AttackType %d in %s"), (int32)TurretRarity, *GetName());
    }

    return Texture;
}

