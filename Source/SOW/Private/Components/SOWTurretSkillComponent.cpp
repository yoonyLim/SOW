// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretSkillComponent.h"

// Gameplay Tags
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

// GAS Component
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

// Core
#include "SOWGameInstance.h"
#include "Manager/USkillManager.h"

#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/SOWCharacter.h"
#include "SOWEnumTypes.h"

// Sets default values for this component's properties
USOWTurretSkillComponent::USOWTurretSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void USOWTurretSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USOWTurretSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USOWTurretSkillComponent::InitializeSkills()
{
	GetSkills();

	for ( const TSharedPtr<FSkillData>& Skill : L_PassiveSkills)
	{
		if (Skill->TriggerType == EPassiveSkillTrigger::OnTurretSpawn)
		{
			TArray<ASOWCharacter*> L_Target = FindTarget(Skill);

			ApplySkill(L_Target, Skill);
		}
	}
}

FGameplayTag USOWTurretSkillComponent::GetElementTagFromOwner()
{
	FString BaseElementStr;

	USOWAbilitySystemComponent* ASC = Cast<ASOWCharacterTurretBase>(GetOwner())->GetSOWAbilitySystemComponent();

	FGameplayTagContainer TagContainer;
	ASC->GetOwnedGameplayTags(TagContainer);

	for (const FGameplayTag& Tag : TagContainer)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Shared.Element")))
		{
			TArray<FString> TagParts;
			Tag.ToString().ParseIntoArray(TagParts, TEXT("."));
			if (TagParts.Num() >= 3)
			{
				BaseElementStr = FString::Printf(TEXT("%s.%s.%s"), *TagParts[0], *TagParts[1], *TagParts[2]);
			}
		}
		break;
	}

	return FGameplayTag::RequestGameplayTag(FName(*BaseElementStr));
}

void USOWTurretSkillComponent::GetSkills()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	UUSkillManager* SM = GI->GetSkillManager();

	L_PassiveSkills = SM->GetUnlockedSkillsByElement(GetElementTagFromOwner());
}

TArray<ASOWCharacter*> USOWTurretSkillComponent::FindTarget(const TSharedPtr<FSkillData>& Skill)
{
	TArray<ASOWCharacter*> L_Targets;

	switch (Skill->TargetType)
	{
	case ESkillTargetType::Self:
	{
		ASOWCharacter* Owner = Cast<ASOWCharacter>(GetOwner());
		L_Targets.Add(Owner);
		break;
	}
	case ESkillTargetType::Enemy:
	{
		break;
	}
	case ESkillTargetType::Ally:
	{
		break;
	}
	case ESkillTargetType::AllyInRange:
	{
		break;
	}
	case ESkillTargetType::EnemyInRange:
	{
		break;
	}
	}

	return L_Targets;
}

void USOWTurretSkillComponent::ApplySkill(TArray<ASOWCharacter*> Targets, const TSharedPtr<FSkillData>& Skill)
{
	switch (Skill->EffectType)
	{
	case ESkillEffectType::CustomScript:
	{
		break;
	}
	case ESkillEffectType::AttributeModifier:
	{
		UGameplayEffect* TempSkill_GE = CreateGameplayEffect(Skill);

		for (ASOWCharacter* Target : Targets)
		{	
			USOWAbilitySystemComponent* Target_ASC = Target->GetSOWAbilitySystemComponent();

			FGameplayEffectContextHandle EffectContext = Target_ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = Target_ASC->MakeOutgoingSpec(TempSkill_GE->GetClass(), 1.f, EffectContext);

			if (SpecHandle.IsValid())
			{
				Target_ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				UE_LOG(LogTemp, Error, TEXT("Apply %s Successfully"), *Skill->SkillID.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Fail to apply %s"), *Skill->SkillID.ToString());
			}
		}
		break;
	}
	}
}

UGameplayEffect* USOWTurretSkillComponent::CreateGameplayEffect(const TSharedPtr<FSkillData>& Skill)
{
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("TempSkillEffect")));

	GE->DurationPolicy = EGameplayEffectDurationType::Instant;

	int32 ModIdx = GE->Modifiers.Num();
	GE->Modifiers.SetNum(ModIdx + 1);

	FGameplayModifierInfo& ModInfo = GE->Modifiers[ModIdx];
	ModInfo.Attribute = Skill->TargetAttribute;
	ModInfo.ModifierOp = Skill->ModifierOp;
	ModInfo.ModifierMagnitude = FScalableFloat(Skill->ModifierValue);

	return (GE);
}