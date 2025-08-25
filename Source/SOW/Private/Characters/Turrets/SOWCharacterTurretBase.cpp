// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"

#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/UI/SOWTurretUIComponent.h"
#include "Components/SOWTurretCombatComponent.h"
#include "Components/SOWTurretEvolutionComponent.h"
#include "Components/SOWTurretSkillComponent.h"
#include "Components/SOWProjectilePoolingComponent.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

#include "GameplayEffectTypes.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"
#include "AIController.h"

#include "AbilitySystem/SOWAttributeSet.h"
#include "Widget/SOWWidgetBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DataAsset/DA_StartupDataTurret.h"


ASOWCharacterTurretBase::ASOWCharacterTurretBase()
{
	check(AttributeSet);

	TurretCombatComponent = CreateDefaultSubobject<USOWTurretCombatComponent>(TEXT("TurretCombatComponent"));

	TurretEvolutionComponent = CreateDefaultSubobject<USOWTurretEvolutionComponent>(TEXT("TurretEvolutionComponent"));

	TurretSkillComponent = CreateDefaultSubobject<USOWTurretSkillComponent>(TEXT("TurretSkillComponent"));

	ProjectilePoolingComponent = CreateDefaultSubobject<USOWProjectilePoolingComponent>(TEXT("TurretProjectilePoolingComponent"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	CharacterType = ESOWCharacterType::Turret;

	TurretUIComponent = CreateDefaultSubobject<USOWTurretUIComponent>(TEXT("TurretUIComponent"));

	DetectionRangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("DetectionRangeDecal"));
	DetectionRangeDecal->SetupAttachment(RootComponent);
	DetectionRangeDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMat(TEXT("/Game/03Materials/M_Range_Decal_Turret.M_Range_Decal_Turret"));
	if (DecalMat.Succeeded())
	{
		DetectionRangeDecal->SetDecalMaterial(DecalMat.Object);
	}
	DetectionRangeDecal->SetVisibility(false);
	
}

void ASOWCharacterTurretBase::BeginPlay()
{
	Super::BeginPlay();

	if (USOWWidgetBase* HealthWidget = Cast<USOWWidgetBase>(HealthWidgetComponent->GetUserWidgetObject())) {
		HealthWidget->InitTurretCreatedWidget(this);
	}

	/*UWorld* World = GetWorld();
	if (World && !GetController())
	{
		AAIController* NewAIController = World->SpawnActor<AAIController>(
			AIControllerClass,
			GetActorLocation(),
			GetActorRotation()
		);

		if (NewAIController)
		{
			NewAIController->Possess(this);
		}
	}*/
}

void ASOWCharacterTurretBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (USOWAbilitySystemComponent* ASC = GetSOWAbilitySystemComponent()) {

		if (!AttributeSet) return;

		ASC->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetDetectionRangeAttribute())
			.AddUObject(this, &ASOWCharacterTurretBase::OnDetectionRangeChanged);


		ASC->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetMaxHealthBaseAttribute())
			.AddUObject(this, &ASOWCharacterTurretBase::OnWidgetAttributeChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetAttackPowerBaseAttribute())
			.AddUObject(this, &ASOWCharacterTurretBase::OnWidgetAttributeChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetDefensePowerBaseAttribute())
			.AddUObject(this, &ASOWCharacterTurretBase::OnWidgetAttributeChanged);

		ASC->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetAttackSpeedBaseAttribute())
			.AddUObject(this, &ASOWCharacterTurretBase::OnWidgetAttributeChanged);

		ASC->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(TEXT("Turret.Status.Buff")),
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &ThisClass::OnGameplayTagChanged);

		ASC->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(TEXT("Turret.Status.Debuff")),
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &ThisClass::OnGameplayTagChanged);
	}
}

void ASOWCharacterTurretBase::InitFromDataAsset()
{
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(StartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (StartupData.IsValid()) {
					UDA_StartupDataBase* LoadData = StartupData.Get();
					UDA_StartupDataTurret* LoadDataTurret = Cast<UDA_StartupDataTurret>(LoadData);
					LoadDataTurret->GiveToAbilitySystemComponent(AbilitySystemComponent);
				}
			}
		)
	);
}



void ASOWCharacterTurretBase::OnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (TurretUIComponent->OnTagChanged.IsBound()) {
		TurretUIComponent->OnTagChanged.Broadcast();
	}
}


void ASOWCharacterTurretBase::SwitchDetectionRangeDecal(bool On)
{
	/*DetectionRangeDecal->SetVisibility(false);

	float radius = GetDetectionRangeRadius();
	DetectionRangeDecal->DecalSize = FVector(radius, radius, radius);
	DetectionRangeDecal->SetVisibility(On);*/

	GetTurretCombatComponent()->VisualizeTurretDetectionRange(On);
}


void ASOWCharacterTurretBase::OnWidgetAttributeChanged(const FOnAttributeChangeData& Data)
{
	FWidgetDesciptableTurretAttribute BaseAtts = TurretCombatComponent->GetWidgetDesciptableTurretAttribute();

	if (Data.Attribute == USOWAttributeSet::GetAttackPowerBaseAttribute())
	{
		// For AttackPower
		TurretUIComponent->OnAttackChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->AttackPowerBase, BaseAtts.AttackPowerBaseValue, BaseAtts.AttackPowerRatioValue));
		
	}
	else if (Data.Attribute == USOWAttributeSet::GetAttackSpeedBaseAttribute())
	{
		// For AttackSpeed
		TurretUIComponent->OnSpeedChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->AttackSpeedBase, BaseAtts.AttackSpeedBaseValue, BaseAtts.AttackSpeedRatioValue));
	}
	else if (Data.Attribute == USOWAttributeSet::GetMaxHealthBaseAttribute())
	{
		// For Health
		TurretUIComponent->OnHealthChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->MaxHealthBase, BaseAtts.MaxHealthBaseValue, BaseAtts.MaxHealthRatioValue));
	}
	else if (Data.Attribute == USOWAttributeSet::GetDefensePowerBaseAttribute())
	{
		// For Defense
		TurretUIComponent->OnDefenseChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->DefensePowerBase, BaseAtts.DefensePowerBaseValue, BaseAtts.DefensePowerRatioValue));
	}
}

void ASOWCharacterTurretBase::InitWidgetAttributeChange()
{
	FWidgetDesciptableTurretAttribute BaseAtts = TurretCombatComponent->GetWidgetDesciptableTurretAttribute();

	TurretUIComponent->OnHealthChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->MaxHealthBase, BaseAtts.MaxHealthBaseValue, BaseAtts.MaxHealthRatioValue));
	TurretUIComponent->OnAttackChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->AttackPowerBase, BaseAtts.AttackPowerBaseValue, BaseAtts.AttackPowerRatioValue));
	TurretUIComponent->OnDefenseChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->DefensePowerBase, BaseAtts.DefensePowerBaseValue, BaseAtts.DefensePowerRatioValue));
	TurretUIComponent->OnSpeedChanged.Broadcast(GetWidgetAttributeChangeDelegate(AttributeSet->AttackSpeedBase, BaseAtts.AttackSpeedBaseValue, BaseAtts.AttackSpeedRatioValue));
}



FWidgetDescAtt ASOWCharacterTurretBase::GetWidgetAttributeChangeDelegate(const FGameplayAttributeData Att, const float Base, const float Circle)
{
	float BaseValue = Att.GetBaseValue();

	float CurrentValue = Att.GetCurrentValue();

	float BuffedValue = CurrentValue - BaseValue;
	float EvoValue = BaseValue - Base - Circle;

	return FWidgetDescAtt(EvoValue, BuffedValue);
}


void ASOWCharacterTurretBase::OnDetectionRangeChanged(const FOnAttributeChangeData& Data)
{
	SwitchDetectionRangeDecal(DetectionRangeDecal->GetVisibleFlag());
}

void ASOWCharacterTurretBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASOWCharacterTurretBase::TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation)
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->TryActivateAbilityWithTag(InAbilityTagToActivation);
}

USOWCharacterUIComponent* ASOWCharacterTurretBase::GetCharacterUIComponent() const
{
	check(TurretUIComponent);
	return TurretUIComponent;
}

USOWTurretUIComponent* ASOWCharacterTurretBase::GetTurretUIComponent() const
{
	check(TurretUIComponent);
	return TurretUIComponent;
}

float ASOWCharacterTurretBase::GetAttackPower() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));
	return AttributeSet->GetAttackPowerBase();
}

float ASOWCharacterTurretBase::GetCurrentHealth() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));
	return AttributeSet->GetCurrentHealth();
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
	return USOWBlueprintFunctionLibrary::EnumToFName<ETurretName>(TurretCombatComponent->GetTurretNameByEnum());
}

FGameplayTag ASOWCharacterTurretBase::GetTurretElementTag() const
{
	//    else if (InTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Shared.Element.Ice")))) {
	checkf(AbilitySystemComponent, TEXT("Invalid Component : AbilitySystemComponent"));

	FGameplayTag ParentTag = FGameplayTag::RequestGameplayTag(TEXT("Shared.Element"));

	// ASC에서 현재 보유한 태그 전부 가져오기
	FGameplayTagContainer OwnedTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);

	// 매칭되는 태그 찾기
	for (const FGameplayTag& Tag : OwnedTags)
	{
		if (Tag.MatchesTag(ParentTag))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FName ASOWCharacterTurretBase::BP_GetTurretName() const
{
	return GetTurretName();
}

float ASOWCharacterTurretBase::BP_GetDetectionRangeRadius() const
{
	return GetDetectionRangeRadius();
}

float ASOWCharacterTurretBase::GetHealthRatio() const
{
	checkf(AttributeSet, TEXT("AttributeSet not Found / Check point : SOWCharacterTurretBase.cpp"));
	return AttributeSet->GetCurrentHealth() / AttributeSet->GetMaxHealthBase();
}

USOWTurretCombatComponent* ASOWCharacterTurretBase::GetTurretCombatComponent() const
{
	checkf(TurretCombatComponent, TEXT("TurretCombatComponent not Found / Check point : SOWCharacterTurretBase.cpp"));
	return TurretCombatComponent;
}

bool ASOWCharacterTurretBase::IsActiveTurret() const 
{
	return TurretCombatComponent->GetActiveBool();
}
USOWTurretSkillComponent* ASOWCharacterTurretBase::GetTurretSkillComponent() const
{
	checkf(TurretSkillComponent, TEXT("TurretSkillComponent not Found / Check point : SOWCharacterTurretBase.cpp"));

	return TurretSkillComponent;
}

USOWProjectilePoolingComponent* ASOWCharacterTurretBase::GetProjectilePoolingComponent() const
{
	checkf(ProjectilePoolingComponent, TEXT("ProjectilePoolingComponent not Found / Check point : SOWCharacterTurretBase.cpp"))
	return ProjectilePoolingComponent;
}




void ASOWCharacterTurretBase::FindTurretByElementTarget()
{
	TArray<AActor*> L_DetectableActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		this->GetActorLocation(),
		this->GetDetectionRangeRadius(),
		ObjectTypes,
		nullptr,
		TArray<AActor*>(),
		L_DetectableActors
	);

	for (AActor* CurrentTarget : L_DetectableActors)
	{
		if (!(CurrentTarget == this))
		{
			ISOWCharacterTypeInterface* SOWCharacter = Cast<ISOWCharacterTypeInterface>(CurrentTarget);
			ESOWCharacterType TargetType = SOWCharacter->GetSOWCharacterType();

			if (TargetType == ESOWCharacterType::Turret)
			{
				USOWTurretSkillComponent* Target_SC = Cast<ASOWCharacterTurretBase>(CurrentTarget)->GetTurretSkillComponent();
				FGameplayTag TargetElemental = Target_SC->GetElementTagFromOwner();
				
				if (TurretSkillComponent->GetElementTagFromOwner() == TargetElemental)
				{
					Target_SC->RiseImpactCount();
				}
			}
		}
	}
}

void ASOWCharacterTurretBase::SetManaConsumption(float value)
{
	GetTurretCombatComponent()->SetManaConsumptionValue(value);
}

	
