// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SOWPlayerSkillComponent.h"
#include "Characters/Player/SOWCharacterPlayer.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWGameInstance.h"
#include "Manager/USkillManager.h"

// Sets default values for this component's properties
USOWPlayerSkillComponent::USOWPlayerSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	PlayerCharacter = Cast<ASOWCharacterPlayer>(GetOwner());
}


// Called when the game starts
void USOWPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USOWPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USOWPlayerSkillComponent::InitializeSkills()
{
	GetSkills();

	
}

FGameplayTag USOWPlayerSkillComponent::GetElementTagFromOwner()
{
	FString BaseElementStr;

	USOWAbilitySystemComponent* ASC = Cast<ASOWCharacterPlayer>(GetOwner())->GetSOWAbilitySystemComponent();

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

void USOWPlayerSkillComponent::GetSkills()
{
	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	UUSkillManager* SM = GI->GetSkillManager();

	for (const TSharedPtr<FSkillData>& SkillPtr : SM->GetUnlockedSkillsByElement(GetElementTagFromOwner()))
	{
		if (SkillPtr->ExecuteType == ESkillExecuteType::Active)
		{
			L_ActiveSkills.Add(SkillPtr);
		}
	}
}

void USOWPlayerSkillComponent::UpdateSkillSelectWidget()
{
	bool bActivateControl = false;
	bool bActivateResonance = false;


	
}
