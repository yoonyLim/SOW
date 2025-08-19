// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SOWStructTypes.h"

#include "Components/ActorComponent.h"
#include "SOWPlayerSkillComponent.generated.h"

class ASOWCharacterPlayer;
class USOWPlayerGameplayAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FGameplayTag CurrentSkillTag;

private:
	TArray<TSharedPtr<FSkillData>> L_ActiveSkills;

	ASOWCharacterPlayer* PlayerCharacter;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	USOWPlayerSkillComponent();

	UFUNCTION(BlueprintCallable)
	void InitializeSkills();

	FGameplayTag GetElementTagFromOwner();

	void GetSkills();

	void UpdateSkillSelectWidget();
};
