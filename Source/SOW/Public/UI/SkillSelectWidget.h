// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SkillSelectWidget.generated.h"

class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class SOW_API USkillSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Icon")
	TMap<FName, TObjectPtr<UTexture2D>> M_SkillIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon", meta = (BindWidget))
	UImage* IMG_MinorSummon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon", meta = (BindWidget))
	UImage* IMG_MajorSummon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon", meta = (BindWidget))
	UImage* IMG_Resonance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon", meta = (BindWidget))
	UImage* IMG_MajorElement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon", meta = (BindWidget))
	UImage* IMG_MinorElement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon", meta = (BindWidget))
	UImage* IMG_Control;

public:
	void InitWidget();

protected:
	virtual void NativeConstruct() override;

};
