// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SOWCharacterEnemyBase.h"

#include "SOWGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

#include "SOWGameplayTags.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "Characters/Enemies/SOWEnemyCombatComponent.h"

#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/Enemies/EnemyIncomingRouteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Structures/Enemies/EnemyStructs.h"
#include "Tile/SOWTileSpawnerActor.h"
#include "Utilities/EnemyIncomingRoute.h"
#include "Widget/Enemy/EnemyHealthBarWidget.h"

#include "AbilitySystem/GA_Enemy_RangedAttack.h"
#include "Manager/OneTimeCurrencyManager.h"

// Sets default values
ASOWCharacterEnemyBase::ASOWCharacterEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterType = ESOWCharacterType::Enemy;

	// EnemyCombatComponent ����
	EnemyCombatComponent = CreateDefaultSubobject<USOWEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	// EnemyIncomingRouteComponent
	EnemyIncomingRouteComponent = CreateDefaultSubobject<UEnemyIncomingRouteComponent>(TEXT("EnemyIncomingRouteComponent"));

	// Set Healthbar Widget
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	if (HealthBarWidget)
	{
		HealthBarWidget->SetupAttachment(RootComponent);
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 1.f));

		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/01Blueprints/UI/Enemy/WBP_EnemyHealthBar") };

		if (WidgetClass.Succeeded())
			HealthBarWidget->SetWidgetClass((WidgetClass.Class));
	}

	// Set Overlay Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OverlayMat(TEXT("/Game/03Materials/Enemy/MI_Enemy_Overlay.MI_Enemy_Overlay"));

	if (OverlayMat.Succeeded())
		GetMesh()->SetOverlayMaterial(OverlayMat.Object);

	// Set Incoming Route
	// GetEnemyIncomingRouteComponent()->SetIncomingRoute(FindClosestIncomingRoute());
}


void ASOWCharacterEnemyBase::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	AbilitySystemComponent->AddLooseGameplayTag(SOWGameplayTags::Enemy_Ability_Initialize);
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
		TargetPriority = EnemyAttributesData->TargetPriority;

		GetCharacterMovement()->MaxWalkSpeed = EnemyAttributesData->MaxWalkSpeed;

		HitAnimation = EnemyAttributesData->HitAnimation;
		DeathAnimation = EnemyAttributesData->DeathAnimation;
		AttackAnimation = EnemyAttributesData->AttackAnimation;

		AttackRadius = EnemyAttributesData->AttackRadius;
		AttackSpeed = EnemyAttributesData->AttackSpeed;
		ShardDropAmount = EnemyAttributesData->ShardDropAmount;
		ShardDropAmountVariation = EnemyAttributesData->ShardDropAmountVariation;
	}

	// Set up HealthBar Widget
	if (!bShouldKeepHealthbarOn)
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
	//AbilitySystemComponent->AddLooseGameplayTag(SOWGameplayTags::Enemy_Ability_Initialize);

	float NewHealth = ASCAttributes->GetMaxHealthBase();
	float MaxHealth = ASCAttributes->GetMaxHealthBase();

	UpdateHealthBarValue(NewHealth, MaxHealth);

	// Set Incoming Route when spawned
	GetEnemyIncomingRouteComponent()->SetIncomingRoute(FindClosestIncomingRoute());

	// 원거리 공격용 어빌리티 추가
	if (AbilitySystemComponent)
	{
		static const TSubclassOf<UGameplayAbility> RangedAttackAbilityClass = UGA_Enemy_RangedAttack::StaticClass();
		FGameplayAbilitySpec AbilitySpec(RangedAttackAbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);

		UE_LOG(LogTemp, Warning, TEXT("[EnemyBase] RangedAttack Ability Granted"));
	}
}

void ASOWCharacterEnemyBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (GetWorldTimerManager().IsTimerActive(HideHealthBarHandle))
		GetWorldTimerManager().ClearTimer(HideHealthBarHandle);

	if (!bShouldKeepHealthbarOn)
	{
		GetWorldTimerManager().SetTimer(
			HideHealthBarHandle,
			FTimerDelegate::CreateLambda([&]() { HealthBarWidget->SetHiddenInGame(true); }),
			1.f,
			false
		);
	}
	
	float NewHealth = Data.NewValue;
	float MaxHealth = ASCAttributes->GetMaxHealthBase();

	if (NewHealth <= 0)
		BroadcastEnemyDeath(0);

	UpdateHealthBarValue(NewHealth, MaxHealth);

	if (!bShouldKeepHealthbarOn)
	{
		HealthBarWidget->SetHiddenInGame(false);

		if (!HealthBarWidget->bHiddenInGame)
			Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject())->PlayFadeAnimation();
	}

	if (HitAnimation)
	{
		// OnHitMontageEnded.BindUObject(this, &ASuraCharacterEnemyBase::OnHitEnded);
		
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(HitAnimation);

		// GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnHitMontageEnded); // montage interrupted
		// GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnHitMontageEnded); // montage ended
	}
}

void ASOWCharacterEnemyBase::SetAIController(AEnemyBaseAIController* NewAIController)
{
	AIController = NewAIController;
	AIController->InitializeBlackBoard(AttackRadius, AttackSpeed, TargetPriority);
}

AEnemyIncomingRoute* ASOWCharacterEnemyBase::FindClosestIncomingRoute() const
{
	// 1. Find the ATileSpawner actor
	ATileSpawner* TileSpawner = nullptr;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileSpawner::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		TileSpawner = Cast<ATileSpawner>(FoundActors[0]);
	}

	if (!TileSpawner)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy '%s': ATileSpawner not found in the world! Cannot find routes."), *GetName());
		return nullptr;
	}

	// 2. Get all available incoming routes from the Tile Spawner
	TArray<AEnemyIncomingRoute*> AvailableRoutes = TileSpawner->GetSpawnedEnemyRoutes();

	if (AvailableRoutes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy '%s': No incoming routes registered by ATileSpawner."), *GetName());
		return nullptr;
	}

	// 3. Find the closest route
	AEnemyIncomingRoute* ClosestRoute = nullptr;
	float MinSquaredDistance = TNumericLimits<float>::Max(); // Initialize with a very large number

	FVector EnemyLocation = GetActorLocation();

	for (AEnemyIncomingRoute* Route : AvailableRoutes)
	{
		if (Route && Route->GetNumberOfPoints() > 0)
		{
			// Get the world position of the first spline point of this route
			FVector RouteStartLocation = Route->GetCurrentIncomingIndexPosition(0);
        
			// Calculate squared distance
			float CurrentSquaredDistance = FVector::DistSquared(EnemyLocation, RouteStartLocation);

			if (CurrentSquaredDistance < MinSquaredDistance)
			{
				MinSquaredDistance = CurrentSquaredDistance;
				ClosestRoute = Route;
			}
		}
	}

	if (ClosestRoute)
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy '%s' chose closest route: '%s' (Squared Distance: %f)"), 
			*GetName(), *ClosestRoute->GetName(), MinSquaredDistance);
	}

	return ClosestRoute;
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
	int ShardAmount = FMath::Clamp(FMath::RandRange(ShardDropAmount - ShardDropAmountVariation, ShardDropAmount + ShardDropAmountVariation) , 0, ShardDropAmount + ShardDropAmountVariation);
	
	OnEnemyDeath.Broadcast(ShardAmount);

	USOWGameInstance* SOWGameInstance = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());

	SOWGameInstance->GetOneTimeCurrencyManager()->AddCurrency(EElementalType::Nature, ShardAmount);
}