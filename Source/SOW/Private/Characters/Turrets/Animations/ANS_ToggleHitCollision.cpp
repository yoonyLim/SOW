// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Turrets/Animations/ANS_ToggleHitCollision.h"
#include "Components/SOWTurretCombatComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Characters/Turrets/Actors/TurretMeleeHitCollision.h"
#include "AbilitySystemBlueprintLibrary.h"

void UANS_ToggleHitCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* OwnerActor = MeshComp->GetOwner();

	ASOWCharacterTurretBase* OwnerTurret = Cast<ASOWCharacterTurretBase>(OwnerActor);
	if (!OwnerTurret) return;

	USOWTurretCombatComponent* OwnerCombatComp = OwnerTurret->GetTurretCombatComponent();
	if (!OwnerCombatComp) return;

	UE_LOG(LogTemp, Warning, TEXT("Collision Enable"));
	OwnerCombatComp->GetHitCollision()->ToggleCollision(true);
}

void UANS_ToggleHitCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AActor* OwnerActor = MeshComp->GetOwner();

	ASOWCharacterTurretBase* OwnerTurret = Cast<ASOWCharacterTurretBase>(OwnerActor);
	if (!OwnerTurret) return;

	USOWTurretCombatComponent* OwnerCombatComp = OwnerTurret->GetTurretCombatComponent();
	if (!OwnerCombatComp) return;

	OwnerCombatComp->GetHitCollision()->ApplyDamageToAllHitTargets();

	OwnerCombatComp->GetHitCollision()->ToggleCollision(false);

	UE_LOG(LogTemp, Warning, TEXT("Collision Disable"));
}