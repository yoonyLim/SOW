// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SOWCharacterUIInterface.h"

// Add default functionality here for any ISOWCharacterUIInterface functions that are not pure virtual.
USOWTurretUIComponent* ISOWCharacterUIInterface::GetTurretUIComponent() const {
	return nullptr;
}

USOWEnemyUIComponent* ISOWCharacterUIInterface::GetEnemyUIComponent() const {
	return nullptr;
}