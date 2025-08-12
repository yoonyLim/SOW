#include "AbilitySystem/GA_Enemy_RangedAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Projectile/Enemy/EnemyProjectileBase.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"

void UGA_Enemy_RangedAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 이벤트 태그 등록
	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Enemy.Ability.Attack"));
	UAbilityTask_WaitGameplayEvent* WaitEvent =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag, nullptr, true, true);

	if (WaitEvent)
	{
		WaitEvent->EventReceived.AddDynamic(this, &UGA_Enemy_RangedAttack::OnAttackEventReceived);
		WaitEvent->ReadyForActivation();
	}
}

void UGA_Enemy_RangedAttack::OnAttackEventReceived(FGameplayEventData Payload)
{
	ASOWCharacterEnemyBase* Enemy = Cast<ASOWCharacterEnemyBase>(GetAvatarActorFromActorInfo());
	if (!Enemy)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const TSubclassOf<AActor> ProjectileClass = Enemy->GetRangedProjectileClass();
	const FName MuzzleSocketName = Enemy->GetMuzzleSocketName();

	if (ProjectileClass)
	{
		FVector SpawnLoc = Enemy->GetActorLocation();
		FRotator SpawnRot = Enemy->GetActorRotation();
		if (Enemy->GetMesh()->DoesSocketExist(MuzzleSocketName))
		{
			SpawnLoc = Enemy->GetMesh()->GetSocketLocation(MuzzleSocketName);
			SpawnRot = Enemy->GetActorForwardVector().Rotation();
		}

		FTransform TM(SpawnRot, SpawnLoc);
		AEnemyProjectileBase* Proj = Enemy->GetWorld()->SpawnActorDeferred<AEnemyProjectileBase>(
			ProjectileClass, TM, Enemy, Enemy, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (Proj)
		{
			Proj->EnemyTypeRowName = Enemy->GetEnemyTypeStr();
			Proj->FinishSpawning(TM);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}