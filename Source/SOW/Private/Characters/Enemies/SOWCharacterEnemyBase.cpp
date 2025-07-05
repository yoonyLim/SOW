// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SOWCharacterEnemyBase.h"

#include "SOWGameplayTags.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structures/Enemies/EnemyStructs.h"
#include "Characters/Enemies/SOWEnemyCombatComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/Enemy/EnemyHealthBarWidget.h"

// Sets default values
ASOWCharacterEnemyBase::ASOWCharacterEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterType = ESOWCharacterType::Enemy;

	// EnemyCombatComponent ����
	EnemyCombatComponent = CreateDefaultSubobject<USOWEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	if (HealthBarWidget)
	{
		HealthBarWidget->SetupAttachment(RootComponent);
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 1.f));

		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/01Blueprints/UI/Enemy/WBP_EnemyHealthBar") };

		if (WidgetClass.Succeeded())
		{
			HealthBarWidget->SetWidgetClass((WidgetClass.Class));
		}
	}
}

// Called when the game starts or when spawned
void ASOWCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	FName LEnemyTypeStr = FName(StaticEnum<EEnemyTypes>()->GetDisplayNameTextByValue(static_cast<int64>(EnemyType)).ToString());
	EnemyTypeStr = LEnemyTypeStr;

	if (const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributeData>(EnemyTypeStr, ""))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, EnemyTypeStr.ToString());

		GetCharacterMovement()->MaxWalkSpeed = EnemyAttributesData->MaxWalkSpeed;

		HitAnimation = EnemyAttributesData->HitAnimation;
		DeathAnimation = EnemyAttributesData->DeathAnimation;
		AttackAnimation = EnemyAttributesData->AttackAnimation;

		AIController = Cast<AEnemyBaseAIController>(GetController());
		
		if (AIController)
			AIController->InitializeBlackBoard(EnemyAttributesData->AttackRadius, EnemyAttributesData->AttackSpeed);
	}

	// Set up HealthBar Widget
	HealthBarWidget->SetHiddenInGame(true);

	// bind to health change event
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			USOWAttributeSet::GetCurrentHealthAttribute()
		).AddUObject(this, &ASOWCharacterEnemyBase::OnHealthChanged);
	}

	// ASC Attributes Reference
	ASCAttributes = Cast<USOWAttributeSet>(AbilitySystemComponent->GetAttributeSet(USOWAttributeSet::StaticClass()));

	// To initialize Game Ability Attribute
	AbilitySystemComponent->AddLooseGameplayTag(SOWGameplayTags::Enemy_Ability_Initialize);
}

void ASOWCharacterEnemyBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (GetWorldTimerManager().IsTimerActive(HideHealthBarHandle))
		GetWorldTimerManager().ClearTimer(HideHealthBarHandle);

	GetWorldTimerManager().SetTimer(
		HideHealthBarHandle,
		FTimerDelegate::CreateLambda([&]() { HealthBarWidget->SetHiddenInGame(true); }),
		1.f,
		false
	);
	
	float NewHealth = Data.NewValue;
	float MaxHealth = ASCAttributes->GetMaxHealthBase();

	UpdateHealthBarValue(NewHealth, MaxHealth);
	HealthBarWidget->SetHiddenInGame(false);

	if (!HealthBarWidget->bHiddenInGame)
		Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject())->PlayFadeAnimation();

	if (HitAnimation)
	{
		// OnHitMontageEnded.BindUObject(this, &ASuraCharacterEnemyBase::OnHitEnded);
		
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(HitAnimation);

		// GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnHitMontageEnded); // montage interrupted
		// GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnHitMontageEnded); // montage ended
	}
}

void ASOWCharacterEnemyBase::UpdateHealthBarValue(float NewHealth, float MaxHealth)
{
	if (UEnemyHealthBarWidget* const Widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
		Widget->SetHealthBarPercent(NewHealth / MaxHealth);
}

void ASOWCharacterEnemyBase::Attack(const ASOWCharacter* TargetActor)
{
	if (AttackAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(AttackAnimation);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack Animation Played!"));
	}
}

void ASOWCharacterEnemyBase::BroadcastEnemyDeath(int GoldAmount)
{
	OnEnemyDeath.Broadcast(GoldAmount);
}

