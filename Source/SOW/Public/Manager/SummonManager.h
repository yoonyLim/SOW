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
	Legendary,
	Origin,
};

UENUM(BlueprintType)
enum class ETurretAttackType : uint8
{
	Melee,
	Range,
	Support,
};

USTRUCT(BlueprintType)
struct FSummonProb : public FTableRowBase {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CommonProb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RareProb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EpicProb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LegendaryProb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OriginProb;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ProfileTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EElementalType ElementalType;

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

	UPROPERTY()
	TArray<FSummonData> L_Legendary;

	UPROPERTY()
	TArray<FSummonData> L_Origin;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnSummonTurret OnSummonTurret;

public:
	void Initialize();

	UFUNCTION(BlueprintCallable)
	bool TurretSummon(FSummonData TurretData);

	UFUNCTION(BlueprintCallable)
	FSummonData RNG(TArray<int32> RarityWeights);

	UFUNCTION(BlueprintCallable)
	FSummonData RNGOriginTurret();

private:
	void InitTurretArray();

};
