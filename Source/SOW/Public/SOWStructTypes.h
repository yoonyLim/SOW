// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "AbilitySystem/Ability/SOWPlayerGameplayAbility.h"
#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "DataAsset/DA_TurretEvolutionData.h"
#include "SOWStructTypes.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerAbilitySet {

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USOWPlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FElementResistanceData : public FTableRowBase {
	GENERATED_BODY()

public:
	FElementResistanceData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Nature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Electro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Death;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Ice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Wave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Divinity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Madness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Resistance Value"))
	float Flame;
};

USTRUCT(BlueprintType)
struct FTurretAttributeData : public FTableRowBase {
	GENERATED_BODY()

public:
	FTurretAttributeData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float MaxHealthBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float MaxHealthRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float DefensePowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float DefensePowerRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float AttackPowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float AttackPowerRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float AttackSpeedBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float AttackSpeedRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float DetectionRange;
};

USTRUCT(BlueprintType)
struct FTurretPropertyData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurretRarity TurretRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETurretTargetSelectionPriority> TurretSettablePriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurretTargetSelectionType TurretTargetSelectionType;
};


USTRUCT(BlueprintType)
struct FTurretData : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TurretName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASOWCharacterTurretBase> TurretClass;
};

USTRUCT(BlueprintType)
struct FTurretEvolutionItem : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UDA_TurretEvolutionData> EvolutionDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EvolutionDescription;
};

USTRUCT(BlueprintType)
struct FTurretEvolutionData : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTurretEvolutionItem> EvolutionAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTurretEvolutionItem> EvolutionBeta;
};