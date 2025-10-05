// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/SOWCharacter.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "GameplayEffectTypes.h"

#include "DataAsset/DA_StartupDataBase.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"

#include "Components/SOWCharacterUIComponent.h"
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

		InitFromDataAsset();

		if (!AbilitySystemComponent || !AttributeSet) return;

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetWalkSpeedAttribute())
			.AddUObject(this, &ASOWCharacter::OnWalkSpeedChanged);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetExtraWalkSpeedAttribute())
			.AddUObject(this, &ASOWCharacter::OnWalkSpeedChanged);
	}
}

void ASOWCharacter::BeginPlay() {

	Super::BeginPlay();
}

void ASOWCharacter::InitFromDataAsset()
{
	if (!StartupData.IsNull()) {
		if (UDA_StartupDataBase* Startup = StartupData.LoadSynchronous()) {
			Startup->GiveToAbilitySystemComponent(AbilitySystemComponent);
		}
	}
}

USOWCharacterUIComponent* ASOWCharacter::GetCharacterUIComponent() const
{
	return nullptr;
}

ESOWCharacterType ASOWCharacter::BP_GetSOWCharacterType() const
{
	return GetSOWCharacterType();
}

void ASOWCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASOWCharacter::OnWalkSpeedChanged(const FOnAttributeChangeData& Data)
{
	

	float NewSpeed = AttributeSet->GetWalkSpeed() * (1.0f + AttributeSet->GetExtraWalkSpeed());
	//UE_LOG(LogTemp, Warning, TEXT("Move Speed Must be Changed"));
	if (GetCharacterMovement()) {
		UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
		UE_LOG(LogTemp, Warning, TEXT("Move Speed Must be Changed Before : %s" ), *FString::SanitizeFloat(MoveComp->MaxWalkSpeed));

		

		MoveComp->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Move Speed Must be Changed After : %s"), *FString::SanitizeFloat(MoveComp->MaxWalkSpeed));

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No CharacterMoveComp Found"));
	}
}

void ASOWCharacter::BP_DeactivateCharacterAllFunctionAsync()
{
	if (!AbilitySystemComponent) return;

	// 어빌리티 제거 (비동기 아님, 즉시 제거)
	AbilitySystemComponent->ClearAllAbilities();

	// 액티브한 GameplayEffect 제거
	FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer()); // 전체 매칭
	AbilitySystemComponent->RemoveActiveEffects(EffectQuery);

	// 애니메이션/사운드/죽음 이펙트 등 재생 (비동기 처리 가능)
	//PlayDeathEffectAsync();

	// 일정 시간 후 제거 (타이머 기반 비동기 처리)
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AActor::K2_DestroyActor, 3.0f, false, 0.1f);
}