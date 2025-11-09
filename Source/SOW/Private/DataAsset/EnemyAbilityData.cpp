// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/EnemyAbilityData.h"
#include "Characters//Enemies/SOWCharacterEnemyBase.h"

EEnemyAbility UEnemyAbilityData::GetEnemyAbility(TSubclassOf<ASOWCharacterEnemyBase> EnemyType)
{
	return EnemyAbility[EnemyType];
}