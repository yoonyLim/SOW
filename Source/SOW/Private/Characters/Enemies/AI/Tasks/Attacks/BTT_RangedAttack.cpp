// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/Tasks/Attacks/BTT_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"           // ASOWCharacterEnemyBase
#include "Projectile/Enemy/EnemyProjectileBase.h"
#include "AbilitySystem/GA_Enemy_RangedAttack.h"
#include "AbilitySystemComponent.h"
//#include "SOWCharacter.h"                    // ASOWCharacter
//#include "SOWCharacterTypeInterface.h"       // ISOWCharacterTypeInterface

UBTT_RangedAttack::UBTT_RangedAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Ranged Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory
)
{
    // 1) AIController & Enemy 캐스팅
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC)
        return EBTNodeResult::Failed;
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Got AIController")); // LogAdded

    ASOWCharacterEnemyBase* Enemy = Cast<ASOWCharacterEnemyBase>(AIC->GetCharacter());
    if (!Enemy)
        return EBTNodeResult::Failed;
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Got EnemyPawn: %s"), *Enemy->GetName()); // LogAdded

    // 2) Blackboard에서 AttackTarget 읽기 & 타입 검사
    UObject* TargetObj = OwnerComp.GetBlackboardComponent()
        ->GetValueAsObject(TEXT("AttackTarget"));
    if (!TargetObj)
        return EBTNodeResult::Failed;
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Found AttackTarget object")); // LogAdded

    ISOWCharacterTypeInterface* SOWIface = Cast<ISOWCharacterTypeInterface>(TargetObj);
    if (!SOWIface)
        return EBTNodeResult::Failed;
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Target implements interface")); // LogAdded

    // 플레이어·터렛만 공격
    ESOWCharacterType TargetType = SOWIface->GetSOWCharacterType();
    if (TargetType != ESOWCharacterType::Player &&
        TargetType != ESOWCharacterType::Turret &&
        TargetType != ESOWCharacterType::CoreRune)
    {
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] TargetType is valid: %s"), *UEnum::GetValueAsString(TargetType)); // LogAdded

    // 3) 실제 ASOWCharacter* 로 캐스팅
    ASOWCharacter* TargetActor = Cast<ASOWCharacter>(TargetObj);
    if (!TargetActor)
        return EBTNodeResult::Failed;
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Got TargetActor: %s"), *TargetActor->GetName()); // LogAdded

    // 4) 공격 몽타주 시작
    bIsAttacking = true;
    Enemy->Attack(TargetActor);
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Called Attack() on Enemy")); // LogAdded


    // 어빌리티 실행.
    UAbilitySystemComponent* ASC = Enemy->FindComponentByClass<UAbilitySystemComponent>();
    if (ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] ASC is valid, checking abilities:"));

        for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
        {
            if (Spec.Ability)
            {
                UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Has ability: %s"), *Spec.Ability->GetName());

                if (Spec.Ability->GetClass() == UGA_Enemy_RangedAttack::StaticClass())
                {
                    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Found matching RangedAttack ability."));
                }
            }
        }

        bool bActivated = ASC->TryActivateAbilityByClass(UGA_Enemy_RangedAttack::StaticClass());
        UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] TryActivateAbilityByClass: %s"),
            bActivated ? TEXT("Success") : TEXT("Failed"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[RangedAttack] ASC is null!"));
    }

    //// 5) 투사체 스폰 로직 삽입

    //const TSubclassOf<AActor> ProjectileClass = Enemy->GetRangedProjectileClass();
    //const FName MuzzleSocketName = Enemy->GetMuzzleSocketName();

    //if (ProjectileClass)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] ProjectileClass valid")); // LogAdded

    //    // 스폰 위치·회전 계산
    //    FVector SpawnLoc = Enemy->GetActorLocation();
    //    FRotator SpawnRot = Enemy->GetActorRotation();
    //    if (Enemy->GetMesh()->DoesSocketExist(MuzzleSocketName))
    //    {
    //        SpawnLoc = Enemy->GetMesh()->GetSocketLocation(MuzzleSocketName);
    //        SpawnRot = (TargetActor->GetActorLocation() - SpawnLoc).Rotation();
    //    }

    //    // Deferred 스폰
    //    FTransform TM(SpawnRot, SpawnLoc);
    //    AEnemyProjectileBase* Proj = Enemy->GetWorld()
    //        ->SpawnActorDeferred<AEnemyProjectileBase>(
    //            ProjectileClass, TM, Enemy, Enemy,
    //            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    //    if (Proj)
    //    {
    //        Proj->EnemyTypeRowName = Enemy->GetEnemyTypeStr();
    //        Proj->FinishSpawning(TM);
    //        UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Spawned projectile")); // LogAdded
    //    }
    //}


    // 6) 몽타주 콜백 바인딩
    if (UAnimInstance* AnimInst = Enemy->GetMesh()->GetAnimInstance())
    {
        OnAttackMontageEnded.BindUObject(this, &UBTT_RangedAttack::OnAttackEnded);
        AnimInst->Montage_SetBlendingOutDelegate(OnAttackMontageEnded);
        AnimInst->Montage_SetEndDelegate(OnAttackMontageEnded);
        UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Bound montage callbacks")); // LogAdded
    }

    // 7) InProgress 반환 → Tick 또는 콜백에서 FinishLatentTask 호출
    FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
    UE_LOG(LogTemp, Warning, TEXT("[RangedAttack] Returning InProgress")); // LogAdded
    return EBTNodeResult::InProgress;
}

void UBTT_RangedAttack::TickTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds
)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 몽타주 콜백에서 bIsAttacking=false로 바뀌면 완료
	if (!bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_RangedAttack::OnAttackEnded(
	UAnimMontage* AnimMontage,
	bool bInterrupted
)
{
	bIsAttacking = false;
}