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
	
}

void ATurretMeleeHitCollision::ToggleCollision(bool bShouldEnable)
{
	if (bShouldEnable) {
		MeleeHitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		MeleeHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
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
	if (!CachedInstigator.Get()) {
		if (!GetInstigator()) return;


		CachedInstigator = Cast<ASOWCharacterTurretBase>(GetInstigator());
		if (!CachedInstigator.Get()) {
			return;
		}
	}

	if (CachedInstigator.Get() == OtherActor || OverlappedActors.Contains(OtherActor)) return;

	ESOWCharacterType TargetType = Cast<ISOWCharacterTypeInterface>(OtherActor)->GetSOWCharacterType();
	if (!USOWBlueprintFunctionLibrary::IsTarget(OwnerPolicy, TargetType)) return;

	UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *OtherActor->GetActorNameOrLabel());

	OverlappedActors.AddUnique(OtherActor);

	FGameplayEventData Data;
	Data.Target = OtherActor;
	//Data.Instigator = CachedInstigator;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		CachedInstigator.Get(),
		SOWGameplayTags::Shared_Event_MeleeHit,
		Data
	);

}

