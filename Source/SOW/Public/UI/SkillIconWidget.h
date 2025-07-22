// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWStructTypes.h"
#include "SkillIconWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class SOW_API USkillIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (BindWidget))
	UButton* BTN_SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bIsUnlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonStyle")
	FLinearColor NormalColor = FLinearColor::White;

public:
	UFUNCTION(BlueprintCallable)
	void OnSkillIconClicked();

	UFUNCTION(BlueprintCallable)
	void UnlockSkillIconWidget();

	UFUNCTION(BlueprintCallable)
	void LockSkillIconWidget();

protected:
	virtual void NativeConstruct();
};
