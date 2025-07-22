// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/Tasks/Attacks/BTT_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SOWCharacterEnemyBase.h"           // ASOWCharacterEnemyBase
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
	// AIController & Enemy 캐스팅
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
		return EBTNodeResult::Failed;

	ASOWCharacterEnemyBase* Enemy = Cast<ASOWCharacterEnemyBase>(AIC->GetCharacter());
	if (!Enemy)
		return EBTNodeResult::Failed;

	// 블랙보드에서 AttackTarget 읽기
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	UObject* TargetObj = BB ? BB->GetValueAsObject("AttackTarget") : nullptr;
	if (!TargetObj)
		return EBTNodeResult::Failed;

	// 인터페이스로 타입 검사 (Player 또는 Turret만 허용)
	ISOWCharacterTypeInterface* SOWIface = Cast<ISOWCharacterTypeInterface>(TargetObj);
	if (!SOWIface)
		return EBTNodeResult::Failed;

	ESOWCharacterType TargetType = SOWIface->GetSOWCharacterType();
	if (TargetType != ESOWCharacterType::Player && TargetType != ESOWCharacterType::Turret)
		return EBTNodeResult::Failed;

	// 실제 ASOWCharacter* 로 캐스팅
	ASOWCharacter* TargetActor = Cast<ASOWCharacter>(TargetObj);
	if (!TargetActor)
		return EBTNodeResult::Failed;

	// 공격 시작
	bIsAttacking = true;
	Enemy->Attack(TargetActor);

	// Enemy 객체로부터 정보 가져오기
	TSubclassOf<AActor> ProjectileClass = Enemy->GetRangedProjectileClass();
	FName MuzzleSocketName = Enemy->GetMuzzleSocketName();


	// =============================== 원거리 공격 로직
	if (TargetActor && ProjectileClass)
	{
		// 투사체 스폰 위치 회전 계산
		FVector SpawnLocation = Enemy->GetActorLocation();
		FRotator SpawnRotation = Enemy->GetActorRotation();

		// 소켓이 있으면 소켓 위치/회전으로 덮어쓰기
		if (Enemy->GetMesh()->DoesSocketExist(MuzzleSocketName))
		{
			SpawnLocation = Enemy->GetMesh()->GetSocketLocation(MuzzleSocketName);
			// 타겟 방향을 향하도록 회전 설정
			SpawnRotation = (TargetActor->GetActorLocation() - SpawnLocation).Rotation();
		}

		// 스폰 파라미터 세팅
		FActorSpawnParameters Params;
		Params.Instigator = Enemy;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Enemy->GetWorld()->SpawnActor<AActor>(
			ProjectileClass, SpawnLocation, SpawnRotation, Params);
	}
	// ==================================

	// 애니메이션 몽타주 끝날 때 까지 대기하도록 델리게이트 바인딩
	if (UAnimInstance* AnimInst = Enemy->GetMesh()->GetAnimInstance())
	{
		OnAttackMontageEnded.BindUObject(this, &UBTT_RangedAttack::OnAttackEnded);
		AnimInst->Montage_SetBlendingOutDelegate(OnAttackMontageEnded);
		AnimInst->Montage_SetEndDelegate(OnAttackMontageEnded);
	}

	// InProgress 상태로 유지
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
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