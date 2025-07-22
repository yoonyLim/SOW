// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWStructTypes.h"
#include "Templates/SharedPointer.h"
#include "GameplayTagContainer.h"
#include "SOWTurretSkillComponent.generated.h"

class ASOWCharacter;
class USOWCharacter;
class UGameplayEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOW_API USOWTurretSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USOWTurretSkillComponent();

	FGameplayTag GetElementTagFromOwner();
	void GetSkills();

	UFUNCTION(BlueprintCallable)
	void InitializeSkills();

	TArray<ASOWCharacter*> FindTarget( const TSharedPtr<FSkillData>& Skill );

	void ApplySkill(TArray<ASOWCharacter*> Targets, const TSharedPtr<FSkillData>& Skill);

	UGameplayEffect* CreateGameplayEffect(const TSharedPtr<FSkillData>& Skill);

private:	
	TArray<TSharedPtr<FSkillData>> L_PassiveSkills;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
