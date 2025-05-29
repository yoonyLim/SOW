// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/SOWCharacter.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "GameplayEffectTypes.h"

#include "DataAsset/DA_StartupDataBase.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASOWCharacter

ASOWCharacter::ASOWCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = CreateDefaultSubobject<USOWAbilitySystemComponent>(TEXT("SOWAbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<USOWAttributeSet>(TEXT("SOWAttributeSet"));
}

ESOWCharacterType ASOWCharacter::GetSOWCharacterType() const
{
	return CharacterType;
}

void ASOWCharacter::PossessedBy(AController* NewController)
{
	
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);



		ensureMsgf(!StartupData.IsNull(), TEXT("Forgot to assign start up data for %s"), *GetName());

		if (!StartupData.IsNull()) {
			if (UDA_StartupDataBase* Startup = StartupData.LoadSynchronous()) {
				Startup->GiveToAbilitySystemComponent(AbilitySystemComponent);
			}
		}

		if (!AbilitySystemComponent || !AttributeSet) return;

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetWalkSpeedAttribute())
			.AddUObject(this, &ASOWCharacter::OnWalkSpeedChanged);
	}
}

void ASOWCharacter::BeginPlay() {

	Super::BeginPlay();
}

void ASOWCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASOWCharacter::OnWalkSpeedChanged(const FOnAttributeChangeData& Data)
{
	float NewSpeed = Data.NewValue;

	if (GetCharacterMovement()) {
	
		UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());

		MoveComp->MaxWalkSpeed = NewSpeed;

	}
}