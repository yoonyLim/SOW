// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWGameInstance.h"
#include "Engine/DataTable.h"
#include "Manager/USkillManager.h"
#include "Manager/GlobalCurrencyManager.h"
#include "Manager/OneTimeCurrencyManager.h"
#include "Manager/SummonManager.h"
#include "UObject/ConstructorHelpers.h"

void USOWGameInstance::Init()
{
	Super::Init();

    SkillManager = NewObject<UUSkillManager>(this);

    if (SkillManager && SkillDataTable)
    {
        SkillManager->Initialize(SkillDataTable);
    }

    GlobalCurrencyManager = NewObject<UGlobalCurrencyManager>(this);

    if (GlobalCurrencyManager)
    {
        GlobalCurrencyManager->Initialize();
    }

    OneTimeCurrencyManager = NewObject<UOneTimeCurrencyManager>(this);

    SummonManager = NewObject<USummonManager>(this);

    if (SummonManager)
    {
        SummonManager->Initialize();
    }
}