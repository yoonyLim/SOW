// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile/ProjectileBase.h"
#include "Structures/Enemies/EnemyStructs.h"         // FEnemyAttributeData 정의 헤더
#include "Engine/DataTable.h"     // UDataTable
#include "EnemyProjectileBase.generated.h"

class ASOWCharacterEnemyBase;

UCLASS()
class SOW_API AEnemyProjectileBase : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyProjectileBase();
	/** 데미지 이펙트 SpecHandle 만 받아서 초기화 */
	//UFUNCTION(BlueprintCallable, Category = "Projectile")
	//void InitializeProjectileSpec(const FGameplayEffectSpecHandle& InDamageSpecHandle);

	/** 에디터에서 DT_EnemyAttributes 에셋을 지정할 변수 */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* EnemyAttributesTable;

	/** 이 투사체가 속한 적 종류(RowName) */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FName EnemyTypeRowName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hit Callback
	virtual void OnCollisionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnCollisionOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
private:

	// BeginPlay() 에서 DataTable에서 읽어 저장할 값들
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	float EffectLevel = 1.f;

#pragma region InternalProperties
	TWeakObjectPtr<ASOWCharacterEnemyBase> CachedInstigator;	// if projectile needs to check instigator several time, it must call GetInstigator() too.
#pragma endregion
};
