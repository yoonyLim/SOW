// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "SOWEnumTypes.h"
#include "PlayerHUD.generated.h"

/**
 * */
UCLASS()
class SOW_API UPlayerHUD : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UProgressBar* HP_Bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UProgressBar* MP_Bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UProgressBar* Stamina_Bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UImage* IMG_Selected_Skill_Icon;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UHorizontalBox* Stacked_Buff_Icon_Box;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UHorizontalBox* Stacked_DeBuff_Icon_Box;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class USkillSelectWidget* SkillSelectWidget;

public:
    void Init(class USOWAbilitySystemComponent* InASC);

    FDelegateHandle HealthChangedHandle;
    FDelegateHandle ManaChangedHandle;
    FDelegateHandle StaminaChangedHandle;

private:
    void BindToASC(class USOWAbilitySystemComponent* InASC);

    void OnHealthChanged(const FOnAttributeChangeData& Data);

    void OnManaChanged(const FOnAttributeChangeData& Data);

    void OnStaminaChanged(const FOnAttributeChangeData& Data);

    void SetProgressBar(EStat ChangedStat, float Max, float Current);

protected:

    UPROPERTY()
    const class USOWAttributeSet* AttributeSetRef;

    UPROPERTY()
    class USOWAbilitySystemComponent* ASC

	virtual void NativeConstruct();

    virtual void NativeDestruct();
};