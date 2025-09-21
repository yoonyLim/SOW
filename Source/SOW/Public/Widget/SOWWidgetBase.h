// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWWidgetBase.generated.h"

class USOWTurretUIComponent;
class USOWEnemyUIComponent;
/**
 * 
 */
UCLASS()
class SOW_API USOWWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Instigator가 별도로 지정되어 생성되는 UI의 경우
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Turret Owning UI Component Initialized"))
	void BP_OnTurretOwningUIComponentInitialized(USOWTurretUIComponent* UIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Enemy Owning UI Component Initialized"))
	void BP_OnEnemyOwningUIComponentInitialized(USOWEnemyUIComponent* UIComponent);

public:

	// Instigator를 별도로 지정하지 않고 생성된 UI의 경우 - 별도 Possess 시점에 처리해야 함.
	UFUNCTION(BlueprintCallable)
	void InitTurretCreatedWidget(AActor* OwningActor);

	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningActor);
};


