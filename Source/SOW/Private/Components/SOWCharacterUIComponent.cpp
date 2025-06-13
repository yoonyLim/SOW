// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWCharacterUIComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"

ASOWCharacterTurretBase* USOWCharacterUIComponent::GetOwnerTurret() const
{
    ASOWCharacterTurretBase* OwnerTurret = Cast<ASOWCharacterTurretBase>(GetOwner());

    if (!OwnerTurret) {
        UE_LOG(LogTemp, Error, TEXT("No Turret has connected"));
    }

    return OwnerTurret;
}
