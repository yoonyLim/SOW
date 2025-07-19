// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "Enumerations/Enemies/EnemyEnums.h"
#include "Interface/EnemyActionsInterface.h"
#include "SOWCharacterEnemyBase.generated.h"

class UEnemyIncomingRouteComponent;
class AEnemyIncomingRoute;
class AEnemyBaseAIController;
class UWidgetComponent;
class UBehaviorTree;
class USOWEnemyCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, int, GoldAmount);

UCLASS()
class SOW_API ASOWCharacterEnemyBase : public ASOWCharacter, public IEnemyActionsInterface
{
	GENERATED_BODY()

	FName EnemyTypeStr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	EEnemyTypes EnemyType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidget;

	FVector2D HealthBarWidgetSize;

	FTimerHandle HideHealthBarHandle;

	UPROPERTY()
	const USOWAttributeSet* ASCAttributes;

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	AEnemyIncomingRoute* FindClosestIncomingRoute() const;

public:
	// Sets default values for this character's properties
	ASOWCharacterEnemyBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "EnemyAttributeData"))
	FDataTableRowHandle EnemyAttributesDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackAnimation;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnEnemyDeath OnEnemyDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool bShouldKeepHealthbarOn = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UpdateHealthBarValue(float NewHealth, float MaxHealth);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AEnemyBaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	//CombatComponent ���� - added by song
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Component")
	USOWEnemyCombatComponent* EnemyCombatComponent;

	// ==================================================== 원거리 공격용 프로퍼티 추가
	// 원거리 투사체
	UPROPERTY(EditAnywhere, Category = "Combat|Ranged")
	TSubclassOf<AActor> RangedProjectileClass;
	
	// 메쉬에 붙인 소켓 이름, 이 위치에서 투사체 발사
	UPROPERTY(EditAnywhere, Category = "Combat|Ranged")
	FName MuzzleSocketName = TEXT("Muzzle");
	// =====================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Incoming Route Component")
	UEnemyIncomingRouteComponent* EnemyIncomingRouteComponent;

public:
	// GETTERS
	FORCEINLINE AEnemyBaseAIController* GetAIController() const { return AIController; };
	FORCEINLINE FVector2D GetHealthBarWidgetSize() const { return HealthBarWidgetSize; } // widget size getter
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; };
	EEnemyTypes GetEnemyType() const { return EnemyType; };
	FName GetEnemyTypeStr() const { return EnemyTypeStr; };
	// GETTERS - added by song
	USOWEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; };
	// 원거리용 프로퍼티에 대한 Getter - added by song
	FORCEINLINE TSubclassOf<AActor> GetRangedProjectileClass() const { return RangedProjectileClass; }
	FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }
	UEnemyIncomingRouteComponent* GetEnemyIncomingRouteComponent() const { return EnemyIncomingRouteComponent; };
	
	virtual void Attack(const ASOWCharacter* TargetActor) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Event")
	void BroadcastEnemyDeath(int GoldAmount);
};
