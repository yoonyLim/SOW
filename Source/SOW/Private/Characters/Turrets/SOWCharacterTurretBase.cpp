// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SOWTurretCombatComponent.h"

#include "Components/WidgetComponent.h"
#include "Widget/SOWWidgetBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "SOWEnumTypes.h"


ASOWCharacterTurretBase::ASOWCharacterTurretBase()
{
	check(AttributeSet);

	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;

	TurretCombatComponent = CreateDefaultSubobject<USOWTurretCombatComponent>(TEXT("TurretCombatComponent"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetMesh());

	CharacterType = ESOWCharacterType::Turret;
	//AttackTarget = nullptr;
}

void ASOWCharacterTurretBase::BeginPlay()
{
	Super::BeginPlay();

	if (USOWWidgetBase* HealthWidget = Cast<USOWWidgetBase>(HealthWidgetComponent->GetUserWidgetObject())) {
		HealthWidget->InitCreatedWidget(this);
	}
}

void ASOWCharacterTurretBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASOWCharacterTurretBase::TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation)
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->TryActivateAbilityWithTag(InAbilityTagToActivation);
}

float ASOWCharacterTurretBase::GetDetectionRangeRadius() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));

	return AttributeSet->GetDetectionRange();
}

float ASOWCharacterTurretBase::GetAttackCooldownTime() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));

	return 1.0f / AttributeSet->GetAttackSpeedBase();
}

FName ASOWCharacterTurretBase::GetTurretName() const
{
	checkf(TurretCombatComponent, TEXT("Invalid Component : TurretCombatComponent"));
	return TurretCombatComponent->TurretName;
}

float ASOWCharacterTurretBase::GetHealthRatio() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));

	return AttributeSet->GetCurrentHealth() / AttributeSet->GetMaxHealthBase();
}


//void ASOWCharacterTurretBase::ActivateTurret()
//{
//	// Callback Function for turret activation  
//	bIsActivated = true;
//	TurretCombatComponent->ActivateTurretCombatSystem();
//}



	
