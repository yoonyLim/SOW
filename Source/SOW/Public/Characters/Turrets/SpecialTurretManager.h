// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"
#include "SOWGameplayTags.h"
#include "UObject/NoExportTypes.h"
#include "SpecialTurretManager.generated.h"

// �ν��Ͻ������� �������ؼ��� �ϵ� ���۷��� �����ϰ� ó���ϰų�, ��������Ʈ�� ������ �� �ִ� ������ ��쿩�� ��ŷ�� �ż� ��Ű¡�� ���Ե�.

class ASOWCharacterTurretSpecialBase;
class ASOWCharacterTurretBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSynergyTurretDelegate, EGlacioStatType, StatType, float, value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSynergyTurretCountChangedDelegate, int, SynergyCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTurretSacrificedStatus, ASOWCharacterTurretBase*, InTurret, float, NewAttack);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurretSacrificedDead, ASOWCharacterTurretBase*, InTurret);
/**
 * 
 */
UCLASS()
class SOW_API USpecialTurretManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioTurret);

	void SummonGlacio();

	void RemoveGlacio();

	// �ó��� ������ ������� �ó��� �߰� �� �ٷ���� ���� ��ȭ�� �����մϴ�.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDelegate OnTurretSummoned;

	// �ó��� ������ ������� �ó��� ���� �� �ٷ���� ���� ��ȭ�� �����մϴ�.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDelegate OnTurretDead;


	// SynergyManager���� ���� �Ӽ� �ͷ� ���� ���� �� ȣ��Ǵ� ��������Ʈ�Դϴ�. �۶�ÿ� ���� �� ������ �����մϴ�.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretCountChangedDelegate OnSynergyChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTurretSacrificedStatus OnTurretSacrificedStatus;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTurretSacrificedDead OnTurretSacrificedDead;

	ASOWCharacterTurretBase* GetGlacio();
protected:
	// �۶�ÿ� �ͷ� ������ ���� Ŭ���� �����Դϴ�.
	TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioClass;

	// �۶�ÿ� �ͷ� ���� �� �ͷ� ������ �����ϴ� �������Դϴ�.
	ASOWCharacterTurretSpecialBase* SummonedGlacio;

	TMap<FGameplayTag, int> AlphaPropertyCondition;
	TMap<FGameplayTag, int> BetaPropertyCondition;

	// �۶�ÿ� ���� �� ���� ó���� ���� �ݹ��Լ��Դϴ�. OnSynergyChanged�� ���ε��Ǿ� �ֽ��ϴ�.
	UFUNCTION()
	void ProcessGlacio(int SynergyCount);

	UFUNCTION()
	void AnnounceSacrificedTurretData(ASOWCharacterTurretBase* InTurret, float NewAttack);

	UFUNCTION()
	void AnnounceSacrificedTurretDead(ASOWCharacterTurretBase* InTurret);

	UFUNCTION(BlueprintCallable)
	void RequestToApplyPropertyCondition(FGameplayTag ConditionTag);

	UFUNCTION(BlueprintCallable)
	void RequestToRemovePropertyCondition(FGameplayTag ConditionTag);
};
