// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "Enumerations/Enemies/EnemyEnums.h"
#include "Interface/EnemyActionsInterface.h"
#include "SOWCharacterEnemyBase.generated.h"

class UGameplayEffect;
class UNiagaraSystem;
class UNiagaraComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	EEnemyTargetPriority TargetPriority;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidget;

	FVector2D HealthBarWidgetSize;

	FTimerHandle HideHealthBarHandle;

	UPROPERTY()
	const USOWAttributeSet* ASCAttributes;

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	float AttackRadius = 0.f;
	float AttackSpeed = 0.f;
	int ShardDropAmount = 0;
	int ShardDropAmountVariation = 0;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilitySystem | DamageEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;

protected:
	// Called when the game starts or when spawned
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	virtual void UpdateHealthBarValue(float NewHealth, float MaxHealth);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AEnemyBaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY()
	UNiagaraSystem* AuraEffect;

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
	EEnemyTargetPriority GetTargetPriority() const { return TargetPriority; };
	FName GetEnemyTypeStr() const { return EnemyTypeStr; };
	// GETTERS - added by song
	USOWEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; };
	// 원거리용 프로퍼티에 대한 Getter - added by song
	FORCEINLINE TSubclassOf<AActor> GetRangedProjectileClass() const { return RangedProjectileClass; }
	FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }

	void SetAIController(AEnemyBaseAIController* NewAIController);

	UFUNCTION(BlueprintCallable)
	UEnemyIncomingRouteComponent* GetEnemyIncomingRouteComponent() const { return EnemyIncomingRouteComponent; };

	UFUNCTION(BlueprintCallable)
	AEnemyIncomingRoute* FindClosestIncomingRoute() const;
	
	virtual void Attack(const ASOWCharacter* TargetActor) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Event")
	void BroadcastEnemyDeath(int GoldAmount);
};
