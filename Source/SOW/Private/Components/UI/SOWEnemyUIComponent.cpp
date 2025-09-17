// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/SOWEnemyUIComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"

ASOWCharacterEnemyBase* USOWEnemyUIComponent::GetOwnerEnemy() const
{
    ASOWCharacterEnemyBase* OwnerEnemy = Cast<ASOWCharacterEnemyBase>(GetOwner());

    if (!OwnerEnemy) {
        UE_LOG(LogTemp, Error, TEXT("No Turret has connected"));
    }

    return OwnerEnemy;
}
