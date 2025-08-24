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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Unusing Value"))
	float MaxHealthBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Unusing Value"))
	float MaxHealthRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Unusing Value"))
	float DefensePowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Unusing Value"))
	float DefensePowerRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float AttackPowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Unusing Value"))
	float AttackPowerRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float AttackSpeedBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Unusing Value"))
	float AttackSpeedRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Attribute Value"))
	float DetectionRange;
};

USTRUCT(BlueprintType)
struct FTurretPropertyData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Turret Property"))
	ETurretRarity TurretRarity;												// Determine Turret Rarity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Turret Property"))
	TArray<ETurretTargetSelectionPriority> TurretSettablePriority;			// Determine What Target should be Selected

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Turret Property"))
	ETurretTargetSelectionPolicy TurretTargetSelectionPolicy;				// Determine What Policy Turret Can Select.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Turret Property"))
	ETurretTargetSelectionType TurretTargetSelectionType;					// Determine How many Target selectable

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATurretProjectileBase> ProjectileToSpawn;					// Determine What To Spawn While Attack

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info"))
	bool HasDependencyOnProjectile;											// Decide whether to pause the ability until the summonned projectile disappears.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info"))
	bool HasProjectileMovement;												// Decide the projectile has movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info"))
	bool HasIndependantCooltime;											// Decide that Turret Cooltime is not based on attribute

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info"))
	float ProjectileLivingTime;												// Determine the projectile lining time

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info", EditCondition = "HasProjectileMovement", EditConditionHides))
	float ProjectileMoveSpeed;												// Determine the projectile move speed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info", EditCondition = "HasIndependantCooltime", EditConditionHides))
	float IndependantCooltime;												// Determine turret ability cooltime independant on attribute

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Projectile Info"))
	float ProjectileScaleRatio;												// Determine the projectile Collision Scale : Default - 1.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Turret Property"))
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
struct FWidgetDescAtt {
	GENERATED_BODY()

	FWidgetDescAtt() {
		EvoValue = 0.f;
		BuffValue = 0.f;
	}
	FWidgetDescAtt(float E, float B) {
		EvoValue = E;
		BuffValue = B;
	}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float EvoValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BuffValue;

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
struct FBuffInfo : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag BuffTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* BuffImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString BuffDescription;
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SkillID;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FGameplayTag SkillElementTag;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	ESkillExecuteType ExecuteType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	EPassiveSkillTrigger TriggerType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	ESkillTargetType TargetType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	ESkillEffectType EffectType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FGameplayTag SkillStateTag;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FName RequiredSkill;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FName UpperLevelSkill;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	int32 RequiredCurrencyAmount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FText SkillDescription;

	/* Trigger Type �� meta data */
	/* In Overlapping Influence Zone */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "TriggerType==EPassiveSkillTrigger::InOverlappingInfluenceZone"));
	uint8 ZoneNum; //

	/* Effect Type �� meta data */
	/* Attribute Modifier */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::AttributeModifier"));
	FGameplayAttribute TargetAttribute;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::AttributeModifier"));
	float ModifierValue = 0.f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::AttributeModifier"))
	TSubclassOf<UGameplayEffect> GEClass;

	/* CustomScript */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::CustomScript"), Category = "Logic Driven")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	/* CircleUpgrade */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "EffectType==ESkillEffectType::CircleUpgrade"), Category = "CircleUpgrade")
	uint8 CircleLevel;
};