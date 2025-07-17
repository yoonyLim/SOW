// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SOWEnemyCombatComponent.h"
#include "Characters/SOWCharacter.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

// Sets default values for this component's properties
USOWEnemyCombatComponent::USOWEnemyCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USOWEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USOWEnemyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USOWEnemyCombatComponent::PerformAttack()
{
	ASOWCharacter* Enemy = Cast<ASOWCharacter>(GetOwner());

	USOWAbilitySystemComponent* ASC = USOWBlueprintFunctionLibrary::NativeGetSOWAbilitySystemComponentFromActorInfo(GetOwner());
	
	ASC->TryActivateAbilityWithTag(AbilityTagToActivation);
}