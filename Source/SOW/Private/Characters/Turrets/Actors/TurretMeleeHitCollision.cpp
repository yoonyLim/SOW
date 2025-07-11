// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/Actors/TurretMeleeHitCollision.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Interface/SOWCharacterTypeInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "SOWGameplayTags.h"

// Sets default values
ATurretMeleeHitCollision::ATurretMeleeHitCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeleeHitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Hit Collision"));
	MeleeHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeleeHitCollision);

	bHasSplashApplied = false;
	
}

void ATurretMeleeHitCollision::ToggleCollision(bool bShouldEnable)
{
	// External Function -> Used in ANS_ToggleHitCollision

	if (bShouldEnable) {
		MeleeHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		MeleeHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
		bHasSplashApplied = false;
	}
}

void ATurretMeleeHitCollision::ApplyDamageToAllHitTargets()
{
	// External Function -> Used in ANS_ToggleHitCollision

	for (AActor* TargetActor : OverlappedActors) {
		FGameplayEventData Data;
		Data.Target = TargetActor;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			CachedInstigator.Get(),
			SOWGameplayTags::Shared_Event_MeleeHit,
			Data
		);
	}
}

// Called when the game starts or when spawned
void ATurretMeleeHitCollision::BeginPlay()
{
	Super::BeginPlay();

	// Set Delegate For Applying Effect to Melee Hit Target
	MeleeHitCollision->OnComponentBeginOverlap.AddDynamic(this, &ATurretMeleeHitCollision::MeleeHit);
}

void ATurretMeleeHitCollision::MeleeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// No Hit Decision Start
	if (!CachedInstigator.Get()) {
		if (!GetInstigator()) return;


		CachedInstigator = Cast<ASOWCharacterTurretBase>(GetInstigator());
		if (!CachedInstigator.Get()) {
			return;
		}
	}

	if (CachedInstigator.Get() == OtherActor || OverlappedActors.Contains(OtherActor)) return;

	if (!OtherActor->Implements<USOWCharacterTypeInterface>()) return;

	ESOWCharacterType TargetType = Cast<ISOWCharacterTypeInterface>(OtherActor)->GetSOWCharacterType();

	if (!USOWBlueprintFunctionLibrary::IsTarget(OwnerPolicy, TargetType)) return;
	// No Hit Decision End

	OverlappedActors.AddUnique(OtherActor);

	// Apply Splash Damage At First Hit
	if (!bHasSplashApplied) {
		bHasSplashApplied = true;

		FGameplayEventData Data;
		Data.Target = this;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			CachedInstigator.Get(),
			SOWGameplayTags::Shared_Event_MeleeHitDone,
			Data
		);
	}
}

