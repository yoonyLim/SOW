// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SOWTurretCombatComponent.h"
#include "Components/SOWTurretEvolutionComponent.h"

#include "Components/WidgetComponent.h"
#include "Widget/SOWWidgetBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "Components/UI/SOWTurretUIComponent.h"
#include "SOWEnumTypes.h"
#include "SOWStructTypes.h"



ASOWCharacterTurretBase::ASOWCharacterTurretBase()
{
	check(AttributeSet);

	TurretCombatComponent = CreateDefaultSubobject<USOWTurretCombatComponent>(TEXT("TurretCombatComponent"));

	TurretEvolutionComponent = CreateDefaultSubobject<USOWTurretEvolutionComponent>(TEXT("TurretEvolutionComponent"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetMesh());

	SettingWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SettingWidgetComponent"));
	SettingWidgetComponent->SetupAttachment(GetMesh());

	CharacterType = ESOWCharacterType::Turret;

	TurretUIComponent = CreateDefaultSubobject<USOWTurretUIComponent>(TEXT("TurretUIComponent"));

}

void ASOWCharacterTurretBase::BeginPlay()
{
	Super::BeginPlay();

	if (USOWWidgetBase* HealthWidget = Cast<USOWWidgetBase>(HealthWidgetComponent->GetUserWidgetObject())) {
		HealthWidget->InitTurretCreatedWidget(this);
	}

	if (USOWWidgetBase* SettingWidget = Cast<USOWWidgetBase>(SettingWidgetComponent->GetUserWidgetObject())) {
		SettingWidget->InitTurretCreatedWidget(this);
	}


}

void ASOWCharacterTurretBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (USOWAbilitySystemComponent* ASC = GetSOWAbilitySystemComponent()) {
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ASOWCharacterTurretBase::OnGameplayEffectAdded);
		ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ASOWCharacterTurretBase::OnGameplayEffectRemoved);
	}
}

void ASOWCharacterTurretBase::OnGameplayEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FEffectOrientedTurretAttribute Data;

	for (const FGameplayModifierInfo& Modifier : SpecApplied.Def->Modifiers)
	{
		const FGameplayAttribute& ModifiedAttr = Modifier.Attribute;

		// 현재 Attribute의 원래 값을 가져옴
		const float OriginalValue = ASC->GetNumericAttributeBase(ModifiedAttr); // Base값 사용

		// Modifier의 Magnitude를 계산
		float ModifierMagnitude = 0.f;
		if (!Modifier.ModifierMagnitude.AttemptCalculateMagnitude(SpecApplied, ModifierMagnitude)) continue;

		float Delta = 0.f;       

		// Modifier Type에 따라 증가량 계산 방식 분기
		switch (Modifier.ModifierOp)
		{
		case EGameplayModOp::Additive:
			Delta = ModifierMagnitude;
			break;

		case EGameplayModOp::Multiplicitive:
			Delta = OriginalValue * (ModifierMagnitude - 1.0f); // 예: 0.5 * (2.0 - 1.0) = +0.5
			break;

		case EGameplayModOp::Override:
			Delta = ModifierMagnitude - OriginalValue;
			break;

		default:
			continue;
		}

		// 속성에 맞게 값 저장
		if (ModifiedAttr == USOWAttributeSet::GetAttackPowerBaseAttribute())
		{
			Data.AttackPowerBaseValue = Delta;
		}
		else if (ModifiedAttr == USOWAttributeSet::GetAttackSpeedBaseAttribute())
		{
			Data.AttackSpeedBaseValue = Delta;
		}
		else if (ModifiedAttr == USOWAttributeSet::GetMaxHealthBaseAttribute())
		{
			Data.MaxHealthBaseValue = Delta;
		}
		else if (ModifiedAttr == USOWAttributeSet::GetDefensePowerBaseAttribute())
		{
			Data.DefensePowerBaseValue = Delta;
		}
	}

	if (TurretUIComponent && TurretUIComponent->OnEffectApplied.IsBound()) {
		TurretUIComponent->OnEffectApplied.Broadcast(Data);
	}
}

void ASOWCharacterTurretBase::OnGameplayEffectRemoved(const FActiveGameplayEffect& Effect)
{
	FEffectOrientedTurretAttribute Data;

	FGameplayEffectSpec RemovedSpec = Effect.Spec;

	for (const FGameplayModifierInfo& Modifier : RemovedSpec.Def->Modifiers)
	{
		const FGameplayAttribute& ModifiedAttr = Modifier.Attribute;

		// Modifier의 Magnitude를 계산
		float Magnitude = 0.f;
		if (!Modifier.ModifierMagnitude.AttemptCalculateMagnitude(RemovedSpec, Magnitude)) continue;

		if (ModifiedAttr == USOWAttributeSet::GetAttackPowerBaseAttribute())
		{
			Data.AttackPowerBaseValue = Magnitude;
		}
		else if (ModifiedAttr == USOWAttributeSet::GetAttackSpeedBaseAttribute())
		{
			Data.AttackSpeedBaseValue = Magnitude;
		}
		else if (ModifiedAttr == USOWAttributeSet::GetMaxHealthBaseAttribute())
		{
			Data.MaxHealthBaseValue = Magnitude;
		}
		else if (ModifiedAttr == USOWAttributeSet::GetDefensePowerBaseAttribute())
		{
			Data.DefensePowerBaseValue = Magnitude;
		}
	}
	if (TurretUIComponent && TurretUIComponent->OnEffectRemoved.IsBound()) {
		TurretUIComponent->OnEffectRemoved.Broadcast(Data);
	}
}

void ASOWCharacterTurretBase::GetModifiedAttributesByGameplayEffects(FEffectOrientedTurretAttribute& BuffData, FEffectOrientedTurretAttribute& DebuffData)
{
	FGameplayEffectQuery Query;
	FEffectOrientedTurretAttribute L_Buff, L_Debuff;

	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = AbilitySystemComponent->GetActiveEffects(Query);

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffectHandles)
	{
		const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(Handle);
		if (ActiveEffect)
		{
			const FGameplayEffectSpec& Spec = ActiveEffect->Spec;

			// 여기서 Spec.Def 또는 SourceTags 등 접근 가능
			//UE_LOG(LogTemp, Log, TEXT("Active Effect: %s"), *GetNameSafe(Spec.Def));

			for (const FGameplayModifierInfo& Modifier : Spec.Def->Modifiers)
			{
				const FGameplayAttribute& ModifiedAttr = Modifier.Attribute;

				// Modifier의 Magnitude를 계산
				float Magnitude = 0.f;
				if (!Modifier.ModifierMagnitude.AttemptCalculateMagnitude(Spec, Magnitude)) continue;

				if (Magnitude > 0) {
					AddBuffData(ModifiedAttr, L_Buff, Magnitude);
				}
				else {
					AddBuffData(ModifiedAttr, L_Debuff, Magnitude);
				}
			}
		}
	}

	BuffData = L_Buff;
	DebuffData = L_Debuff;
}

void ASOWCharacterTurretBase::AddBuffData(const FGameplayAttribute& ModifiedAttr, FEffectOrientedTurretAttribute& Data, float Value)
{
	if (ModifiedAttr == USOWAttributeSet::GetAttackPowerBaseAttribute())
	{
		Data.AttackPowerBaseValue += Value;
	}
	else if (ModifiedAttr == USOWAttributeSet::GetAttackSpeedBaseAttribute())
	{
		Data.AttackSpeedBaseValue += Value;
	}
	else if (ModifiedAttr == USOWAttributeSet::GetMaxHealthBaseAttribute())
	{
		Data.MaxHealthBaseValue += Value;
	}
	else if (ModifiedAttr == USOWAttributeSet::GetDefensePowerBaseAttribute())
	{
		Data.DefensePowerBaseValue += Value;
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

USOWCharacterUIComponent* ASOWCharacterTurretBase::GetCharacterUIComponent() const
{
	return TurretUIComponent;
}

USOWTurretUIComponent* ASOWCharacterTurretBase::GetTurretUIComponent() const
{
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




	
