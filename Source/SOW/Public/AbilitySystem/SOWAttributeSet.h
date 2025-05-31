// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWStructTypes.h"
#include "SOWAttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ISOWCharacterUIInterface;

UCLASS()
class SOW_API USOWAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public :
	USOWAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

		/* Status Health */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealthBase;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, MaxHealthBase)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealthRatio;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, MaxHealthRatio)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, CurrentHealth)

		/* Status Attack */
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FGameplayAttributeData AttackPowerBase;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, AttackPowerBase)

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FGameplayAttributeData AttackPowerRatio;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, AttackPowerRatio)

		/* Status Defense */
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData DefensePowerBase;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, DefensePowerBase)

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData DefensePowerRatio;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, DefensePowerRatio)

		/* Status Interaction */
    UPROPERTY(BlueprintReadOnly, Category = "Range")
	FGameplayAttributeData DetectionRange;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, DetectionRange)

	UPROPERTY(BlueprintReadOnly, Category = "Speed")
	FGameplayAttributeData AttackSpeedBase;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, AttackSpeedBase)

	UPROPERTY(BlueprintReadOnly, Category = "Speed")
	FGameplayAttributeData AttackSpeedRatio;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, AttackSpeedRatio)

	UPROPERTY(BlueprintReadOnly, Category = "Speed")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, WalkSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageOverTime;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, DamageOverTime)

		/* Status Elemental Resistance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData NatureResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, NatureResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData ElectroResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, ElectroResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData DeathResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, DeathResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, IceResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData WaveResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, WaveResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData DivinityResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, DivinityResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData MadnessResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, MadnessResistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	FGameplayAttributeData FlameResistance;
	ATTRIBUTE_ACCESSORS(USOWAttributeSet, FlameResistance)

	



	/** 특정 태그에 대한 저항값을 반환하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Resistance")
	float GetResistanceForElementWithElementTag(FGameplayTag ElementTag) const;

private:
	TWeakInterfacePtr<ISOWCharacterUIInterface> CachedCharacterUIInterface;

};
