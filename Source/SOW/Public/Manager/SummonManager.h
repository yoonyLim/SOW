// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SOWStructTypes.h"
#include "SOWEnumTypes.h"
#include "SummonManager.generated.h"
/**
 * 
 */

class ASOWCharacterTurretBase;
class UDataTable;


UENUM(BlueprintType)
enum class ERarity : uint8
{
	// added by : HYJ
	Common,
	Rare,
	Epic,
};

UENUM(BlueprintType)
enum class ETurretAttackType : uint8
{
	Melee,
	Range,
	Support,
};

USTRUCT(BlueprintType)
struct FSummonData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TurretName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurretAttackType TurretAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASOWCharacterTurretBase> TurretClass;

	bool operator==(const FSummonData& Other) const;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSummonTurret, const FSummonData&, TurretToSummon);

UCLASS()
class SOW_API USummonManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UDataTable* DT_SummonTurretProb;

	UPROPERTY()
	TArray<FSummonData> L_Common;

	UPROPERTY()
	TArray<FSummonData> L_Rare;

	UPROPERTY()
	TArray<FSummonData> L_Epic;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnSummonTurret OnSummonTurret;

public:
	void Initialize();

	UFUNCTION(BlueprintCallable)
	bool TurretSummon();

private:
	void InitTurretArray();

	FSummonData RNG();
};
