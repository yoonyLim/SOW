// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Enemy/EnemyProjectileBase.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"

AEnemyProjectileBase::AEnemyProjectileBase()
{
	
}

void AEnemyProjectileBase::BeginPlay()
{
	// Super::BeginPlay();
}

void AEnemyProjectileBase::OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BP_PostProjectileHit();
}
