// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWEnumTypes.h"
#include "SOWGameplayTags.h"
#include "UObject/NoExportTypes.h"
#include "SpecialTurretManager.generated.h"

// 인스턴스에서는 어지간해서는 하드 레퍼런스 가능하게 처리하거나, 블루프린트로 접근할 수 있는 에셋인 경우여야 쿠킹이 돼서 패키징에 포함됨.

class ASOWCharacterTurretSpecialBase;
class ASOWCharacterTurretBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSynergyTurretDelegate, EGlacioStatType, StatType, float, value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSynergyTurretCountChangedDelegate, int, SynergyCount);
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

	// 시너지 개수와 상관없이 시너지 추가 시 다뤄야할 스텟 강화에 관여합니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDelegate OnTurretSummoned;

	// 시너지 개수와 상관없이 시너지 제거 시 다뤄야할 스텟 강화에 관여합니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretDelegate OnTurretDead;


	// SynergyManager에서 얼음 속성 터렛 개수 변경 시 호출되는 델리게이트입니다. 글라시오 생성 및 죽음에 관여합니다.
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSynergyTurretCountChangedDelegate OnSynergyChanged;

	ASOWCharacterTurretBase* GetGlacio();
protected:
	// 글라시오 터렛 생성을 위한 클래스 에셋입니다.
	TSubclassOf<ASOWCharacterTurretSpecialBase> GlacioClass;

	// 글라시오 터렛 생성 시 터렛 정보를 보관하는 포인터입니다.
	ASOWCharacterTurretSpecialBase* SummonedGlacio;

	TMap<FGameplayTag, int> AlphaPropertyCondition;
	TMap<FGameplayTag, int> BetaPropertyCondition;

	// 글라시오 생성 및 제거 처리를 위한 콜백함수입니다. OnSynergyChanged에 바인딩되어 있습니다.
	UFUNCTION()
	void ProcessGlacio(int SynergyCount);

	UFUNCTION(BlueprintCallable)
	void RequestToApplyPropertyCondition(FGameplayTag ConditionTag);

	UFUNCTION(BlueprintCallable)
	void RequestToRemovePropertyCondition(FGameplayTag ConditionTag);
};
