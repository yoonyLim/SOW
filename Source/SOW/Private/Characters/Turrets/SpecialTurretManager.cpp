// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SpecialTurretManager.h"

void USpecialTurretManager::Initialize() {
	FStringClassReference GlacioBPRef(TEXT("/Game/Blueprints/Turret/03Ice/00Glacio/BP_Turret_Special_Glacio"));
	if (UClass* LoadedClass = GlacioBPRef.TryLoadClass<ASOWCharacterTurretSpecialBase>())
	{
		GlacioClass = LoadedClass;
	}
}
