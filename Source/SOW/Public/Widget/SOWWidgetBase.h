// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOWWidgetBase.generated.h"

class USOWCharacterUIComponent;
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

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning UI Component Initialized"))
	void BP_OnOwningUIComponentInitialized(USOWCharacterUIComponent* UIComponent);

public:

	// Instigator를 별도로 지정하지 않고 생성된 UI의 경우 - 별도 Possess 시점에 처리해야 함.
	UFUNCTION(BlueprintCallable)
	void InitCreatedWidget(AActor* OwningActor);
};


