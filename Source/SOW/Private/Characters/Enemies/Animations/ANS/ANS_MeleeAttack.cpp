// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Animations/ANS/ANS_MeleeAttack.h"

#include "Characters/Enemies/SOWCharacterEnemyBase.h"
#include "Components/CapsuleComponent.h"

void UANS_MeleeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EvetnRef);

	Enemy = Cast<ASOWCharacterEnemyBase>(MeshComp->GetOwner());
}

void UANS_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration, EvetnRef);

	if (Enemy && MeshComp)
	{
		FHitResult Hit;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetEnemyChar());

		const FVector Start = GetEnemyChar()->GetActorLocation() + GetEnemyChar()->GetActorForwardVector() * GetEnemyChar()->GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetEnemyChar()->GetActorForwardVector() * AttackRange;

		// ANS cannot access GetWorld directly! Must be accessed through MeshComp or whoever has access to GetWorld function first
		bool bHit = MeshComp->GetWorld()->SweepSingleByChannel(
			Hit,
			Start,
			End,
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeSphere(AttackRadius),
			CollisionQueryParams
		);

		if (bHit && bCanInflictDamage)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("poyo"));

			if (ASOWCharacterPlayer* const Player = Cast<ASOWCharacterPlayer>(Hit.GetActor()))
			{

				/*FDamageData DamageData;
				DamageData.DamageAmount = Enemy->GetAttackDamageAmount() + AdditionalDamageAmount;
				DamageData.DamageType = EDamageType::Melee;

				Player->TakeDamage(DamageData, GetEnemyChar());*/

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("poyo"));

				bCanInflictDamage = false;
			}
		}

		// ANS cannot access GetWorld directly
		DrawDebugSphere(MeshComp->GetWorld(), Start + (End - Start) * 0.5f, AttackRadius, 24, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	}
}

void UANS_MeleeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyEnd(MeshComp, Animation, EvetnRef);

	bCanInflictDamage = true;
}
