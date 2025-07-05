// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SOWWidgetBase.h"
#include "Interface/SOWCharacterUIInterface.h"
#include "Components/SOWCharacterUIComponent.h"

void USOWWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*UE_LOG(LogTemp, Warning, TEXT("UI Initialzed"));

	if (ISOWCharacterUIInterface* UIInterface = Cast<ISOWCharacterUIInterface>(GetOwningPlayerPawn())) {

		if (USOWCharacterUIComponent* UIComponent = UIInterface->GetCharacterUIComponent()) {
			BP_OnOwningUIComponentInitialized(UIComponent);

			UE_LOG(LogTemp, Warning, TEXT("BP_OnOwningUIComponentInitialized was activated"));
		}
	}*/
}

void USOWWidgetBase::InitTurretCreatedWidget(AActor* OwningActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Extracted UI Component for %s"), *OwningActor->GetActorNameOrLabel());

	if (ISOWCharacterUIInterface* UIInterface = Cast<ISOWCharacterUIInterface>(OwningActor)) {
		USOWTurretUIComponent* UIComponent = UIInterface->GetTurretUIComponent();

		checkf(UIComponent, TEXT("Failed to Extract UI Component for %s"), *OwningActor->GetActorNameOrLabel());

		BP_OnTurretOwningUIComponentInitialized(UIComponent);
	}
}
