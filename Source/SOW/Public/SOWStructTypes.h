// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "AbilitySystem/Ability/SOWPlayerGameplayAbility.h"
#include "AbilitySystem/Ability/SOWTurretGameplayAbility.h"
#include "DataAsset/DA_TurretEvolutionData.h"

#include "SOWEnumTypes.h"
#include "SOWStructTypes.generated.h"

class ASOWCharacterTurretBase;
class ATurretProjectileBase;
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
	ETurretRarity TurretRarity;												// Determine Turret Rarity

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETurretTargetSelectionPriority> TurretSettablePriority;			// Determine What Target should be Selected

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy;				// Determine What Policy Turret Can Select.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurretTargetSelectionType TurretTargetSelectionType;					// Determine How many Target selectable

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATurretProjectileBase> ProjectileToSpawn;					// Determine What To Spawn While Attack

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasDependencyOnProjectile;											// Decide whether to pause the ability until the summonned projectile disappears.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasProjectileMovement;												// Decide the projectile has movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasIndependantCooltime;											// Decide that Turret Cooltime is not based on attribute

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileLivingTime;												// Determine the projectile lining time

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "HasProjectileMovement", EditConditionHides))
	float ProjectileMoveSpeed;												// Determine the projectile move speed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "HasIndependantCooltime", EditConditionHides))
	float IndependantCooltime;												// Determine turret ability cooltime independant on attribute

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileScaleRatio;												// Determine the projectile Collision Scale : Default - 1.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetSelectCount;												// Determine How many targets must be selected in the turret
};


USTRUCT(BlueprintType)
struct FSpellCombination
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicSpell FirstSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicSpell SecondSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicSpell ThirdSpell;

	bool operator==(const FSpellCombination& Other) const
	{
		return FirstSpell == Other.FirstSpell &&
			SecondSpell == Other.SecondSpell &&
			ThirdSpell == Other.ThirdSpell;
	}
};

USTRUCT(BlueprintType)
struct FTurretSummonData : public FTableRowBase {

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpellCombination SpellComb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TurretName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASOWCharacterTurretBase> TurretClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;
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
struct FWidgetDesciptableTurretAttribute {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealthBaseValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealthRatioValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefensePowerBaseValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefensePowerRatioValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackPowerBaseValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackPowerRatioValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackSpeedBaseValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackSpeedRatioValue = 0.f;
};

USTRUCT(BlueprintType)
struct FEffectOrientedTurretAttribute {
	GENERATED_BODY()

public:
	FEffectOrientedTurretAttribute() {
		MaxHealthBaseValue = 0.f;
		DefensePowerBaseValue = 0.f;
		AttackPowerBaseValue = 0.f;
		AttackPowerBaseValue = 0.f;
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealthBaseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefensePowerBaseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackPowerBaseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackSpeedBaseValue;
};

USTRUCT(BlueprintType)
struct FTurretEvolutionData : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTurretEvolutionItem> EvolutionAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTurretEvolutionItem> EvolutionBeta;
};

USTRUCT(BlueprintType)
struct FCircleWeight
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CircleLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;
};

USTRUCT(BlueprintType)
struct FCircleWeightByLevelRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCircleWeight> CircleWeights;
};

USTRUCT(BlueprintType)
struct FMagicSpell : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMagicSpell MagicSpell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* SanskritImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
};

USTRUCT(BlueprintType)
struct FStepEffectData
{
	GENERATED_BODY()

	/** 효과의 타입 */
	UPROPERTY(EditDefaultsOnly)
	ESkillEffectType EffectType;

	/** 해당 효과를 구분하거나 처리 로직을 연결할 태그 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SkillEffectTag;

	/** 수치 변화값 (AttributeModifier일 때만 유의미) */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::AttributeModifier"))
	float Value = 0.0f;

	/** 수치 연산 방식 (Add, Multiply 등) (AttributeModifier일 때만 유의미) */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::AttributeModifier"))
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;
};

USTRUCT(BlueprintType)
struct FSkillStepData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SkillID;

	UPROPERTY(EditDefaultsOnly)
	int32 StepLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SkillElementTag;

	/* AttributeModifier 타입일 경우 사용할 효과 목록 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FStepEffectData> Effects;

	/* 스킬 활성화 시 부여될 게임플레이 태그 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GrantTag;

	/* 선행 스킬 ID */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RequiredTags;

	UPROPERTY(EditDefaultsOnly)
	int32 RequiredCurrencyAmount = 0;

	UPROPERTY(EditDefaultsOnly)
	FText SkillDescription;
};
