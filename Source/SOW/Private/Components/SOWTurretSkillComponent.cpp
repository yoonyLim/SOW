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

	UE_LOG(LogTemp, Error, TEXT("%s"), *BaseElementStr);

	return FGameplayTag::RequestGameplayTag(FName(*BaseElementStr));
}

void USOWTurretSkillComponent::GetSkills()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	UUSkillManager* SM = GI->GetSkillManager();

	FGameplayTag NatureTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element.Nature"));

	L_PassiveSkills = SM->GetUnlockedSkillsByElement(NatureTag);
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
		for (ASOWCharacter* Target : Targets)
		{
			if (!IsValid(Target)) continue;

			USOWAbilitySystemComponent* ASC = Target->GetSOWAbilitySystemComponent();
			if (!ASC) continue;

			FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(
				FGameplayAbilitySpec(Skill->GameplayAbilityClass, 1, INDEX_NONE, this));

			// ½ÇÇà
			bool bSuccess = ASC->TryActivateAbility(Handle);

			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("GA [%s] activated for Target: %s"),
					*Skill->GameplayAbilityClass->GetName(), *Target->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to activate GA [%s] for Target: %s"),
					*Skill->GameplayAbilityClass->GetName(), *Target->GetName());
			}
		}
		break;
	}
	case ESkillEffectType::AttributeModifier:
	{
		if (!Skill.IsValid() || !Skill->TargetAttribute.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid SkillData"));
			return;
		}

		TSubclassOf<UGameplayEffect> SelectedGEClass = nullptr;
		SelectedGEClass = Skill->GEClass;

		if (!SelectedGEClass)
		{
			UE_LOG(LogTemp, Error, TEXT("GEClass not assigned"));
			return;
		}

		for (ASOWCharacter* Target : Targets)
		{
			USOWAbilitySystemComponent* ASC = Target->GetSOWAbilitySystemComponent();
			if (!ASC) continue;

			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(SelectedGEClass, 1.f, Context);

			if (!SpecHandle.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid GE Spec"));
				continue;
			}

			UE_LOG(LogTemp, Warning, TEXT("=== Debugging GE Application ==="));
			UE_LOG(LogTemp, Warning, TEXT("SkillID: %s"), *Skill->SkillID.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Target Attribute: %s"), *Skill->TargetAttribute.GetName());
			UE_LOG(LogTemp, Warning, TEXT("GEClass: %s"), *SelectedGEClass->GetName());
			UE_LOG(LogTemp, Warning, TEXT("SetByCaller Value: %f"), Skill->ModifierValue);

			UGameplayEffect* GE_CDO = SelectedGEClass->GetDefaultObject<UGameplayEffect>();
			if (GE_CDO)
			{
				UE_LOG(LogTemp, Warning, TEXT("GE Modifier Count: %d"), GE_CDO->Modifiers.Num());

				for (const auto& Mod : GE_CDO->Modifiers)
				{
					UE_LOG(LogTemp, Warning, TEXT("Modifier Attribute : % s"), *Mod.Attribute.AttributeName);
				}
			}

			FGameplayTag SetByCallerTag = FGameplayTag::RequestGameplayTag("Data.ModValue");
			SpecHandle.Data->SetSetByCallerMagnitude(SetByCallerTag, Skill->ModifierValue);

			FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
			if (Handle.IsValid())
			{
				float NewVal = ASC->GetNumericAttribute(Skill->TargetAttribute);
				UE_LOG(LogTemp, Warning, TEXT("Skill %s applied. New [%s] = %f"),
					*Skill->SkillID.ToString(), *Skill->TargetAttribute.GetName(), NewVal);
			}
		}
		break;
	}
	}
}
