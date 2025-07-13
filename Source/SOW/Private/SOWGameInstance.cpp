// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWGameInstance.h"
#include "Engine/DataTable.h"
#include "Manager/USkillManager.h"
#include "UObject/ConstructorHelpers.h"

void USOWGameInstance::Init()
{
	Super::Init();

    SkillManager = NewObject<UUSkillManager>(this);

    if (SkillManager && SkillDataTable)
    {
        SkillManager->Initialize(SkillDataTable);
    }
}