// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWGameplayTags.h"
#include "SOWCharacterTurretBase.generated.h"

class UCapsuleComponent;
class USOWTurretCombatComponent;
class USOWTurretEvolutionComponent;
class USOWTurretSkillComponent;
class USOWProjectilePoolingComponent;
class UWidgetComponent;
struct FEffectOrientedTurretAttribute;
struct FWidgetDescAtt;
class UDecalComponent;
class ATileBase;



/**
 * 
 */
UCLASS()
class SOW_API ASOWCharacterTurretBase : public ASOWCharacter
{
	GENERATED_BODY()
	
public:
	ASOWCharacterTurretBase();

	virtual void Tick(float DeltaTime) override;

	void TryActivateAbilityWithTagOnASC(const FGameplayTag& InAbilityTagToActivation);

	/* Begin ISOWCharacterUIInterface implement */
	virtual USOWCharacterUIComponent* GetCharacterUIComponent() const override;
	virtual USOWTurretUIComponent* GetTurretUIComponent() const override;
	/* End ISOWCharacterUIInterface implement */

	UFUNCTION(BlueprintCallable)
	void SwitchDetectionRangeDecal(bool On, TArray<ATileBase*>& OutTiles);

	UFUNCTION(BlueprintCallable)
	void FindTurretByElementTarget();

	UFUNCTION(BlueprintCallable)
	void SellTurret(float Price);

	void SwitchCollision(bool bActive);



#pragma region NativeGetter

	float GetAttackPower() const;
	float GetAttackSpeed() const;
	float GetDetectionRangeRadius() const;										// Get Attack Radius From Attribute Set in Turret Base 
	float GetAttackCooldownTime() const;										// Get Attack Delay From Attribute Set in Turret Base
	int32 GetCircleCount() const { return CircleCount; };						// Get Circle Count when Turret Spawning Time
	FName GetTurretName() const;												// Get Turret Name (enum) to FName
	FName GetTurretRank() const;
	FGameplayTag GetTurretElementTag() const;

#pragma endregion


#pragma region BPGetter

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Name"))
	FName BP_GetTurretName() const;												// Get Turret Name in FName at Blueprint

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Rank"))
	FName BP_GetTurretRank() const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Detection Range"))
	float BP_GetDetectionRangeRadius() const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Attack Speed"))
	float BP_GetAttackSpeed() const;												// Get Health Ratio From Attribute Set in Turret Base for setting UI

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Turret Attack Power"))
	float BP_GetAttackPower() const;

	UFUNCTION(BlueprintPure)
	USOWTurretCombatComponent* GetTurretCombatComponent() const;				// Get Combat Component at other classes

	UFUNCTION(BlueprintPure)
	bool IsActiveTurret() const;

	UFUNCTION(BlueprintPure)
	USOWTurretSkillComponent* GetTurretSkillComponent() const;				// Get Skill Component at other classes

	UFUNCTION(BlueprintPure)
	USOWProjectilePoolingComponent* GetProjectilePoolingComponent() const;				// Get Pooling Component at other classes

#pragma endregion

#pragma region BlueprintFunctions
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Bind On Target Dead"))
	void BP_BindOnTargetDead(AActor* Target);
#pragma endregion

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Turret|Properties", meta = (ExposeOnSpawn = true))
	int32 CircleCount;															// Determine additional Attribute. multiply with attribute - ratio and add in base


	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitFromDataAsset() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* CustomTurretStatusfWidget; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretCombatComponent* TurretCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretEvolutionComponent* TurretEvolutionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWTurretSkillComponent* TurretSkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USOWProjectilePoolingComponent* ProjectilePoolingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	USOWTurretUIComponent* TurretUIComponent;

private:

	UFUNCTION()
	void OnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount);

	void OnDetectionRangeChanged(const FOnAttributeChangeData& Data);

	void OnWidgetAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable)
	void InitWidgetAttributeChange();

	FWidgetDescAtt GetWidgetAttributeChangeDelegate(const FGameplayAttributeData Att, const float Base, const float Circle);
};
