// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/SOWTurretUIComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"

ASOWCharacterTurretBase* USOWTurretUIComponent::GetOwnerTurret() const
{
    ASOWCharacterTurretBase* OwnerTurret = Cast<ASOWCharacterTurretBase>(GetOwner());

    if (!OwnerTurret) {
        UE_LOG(LogTemp, Error, TEXT("No Turret has connected"));
    }

    return OwnerTurret;
}