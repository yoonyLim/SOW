// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SynergyInfoWidget.h"

#include "Components/Image.h"

#include "Manager/TurretSynergyManager.h"
#include "Manager/Actor/SynergyUpdateAnnouncer.h"
#include "SOWGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USynergyInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	USOWGameInstance* GI = Cast<USOWGameInstance>(GetWorld()->GetGameInstance());
	SUA = GI->GetTurretSynergyManager()->GenerateSynergyUpdateAnnouncer();

	//SUA->OnSynergyUpdated.AddDynamic(this, &USynergyInfoWidget::UpdateSynergyInfoWidget);

	//for (const TPair<FName, UImage*>& Elem : TurretPortraits)
	//{
	//	FLinearColor& OriginalColor = TurretPortraitsColor.FindOrAdd(Elem.Key, Elem.Value->ColorAndOpacity);
	//	FLinearColor NewColor = OriginalColor * 0.4f;
	//	Elem.Value->SetColorAndOpacity(NewColor);
	//}
}

void USynergyInfoWidget::UpdateSynergyInfoWidget(EElementalType ElementType, int SynergyCount, const TArray<FName>& SynergyComposer)
{
	Count = SynergyCount;

	UpdateDetailContainer();
	UpdateSynergyCountIcon();
	//UpdateTurretPortrait(SynergyComposer);
}

void USynergyInfoWidget::UpdateDetailContainer()
{

	for (int i = 1; i < MaxCount; i++)
	{
		if (i <= Count)
		{
			DetailContainers[i - 1]->SetBrushTintColor(FColor(1.0f, 0.7f, 0.5f));
		}
		else
		{
			DetailContainers[i - 1]->SetBrushTintColor(FColor(0.4f, 0.4f, 0.4f));
		}
	}
}

void USynergyInfoWidget::UpdateSynergyCountIcon()
{
	UE_LOG(LogTemp, Warning, TEXT("Yeya"));
	if (SynergyCountIcons.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sibal"));
		return;
	}


	for (int i = 1; i < MaxCount; i++)
	{
		if (i <= Count)
		{
			UE_LOG(LogTemp, Warning, TEXT("sex"));
			SynergyCountIcons[i -1]->SetBrushFromTexture(ActivateSynergyCountIcon);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("fuck"));
			SynergyCountIcons[i-1]->SetBrushFromTexture(DeactivateSynergyCountIcon);
		}
	}
}

//void USynergyInfoWidget::UpdateTurretPortrait(const TArray<FName>& SynergyComposer)
//{
//	for (const TPair<FName, UImage*>& Elem : TurretPortraits)
//	{
//		if (SynergyComposer.Contains(Elem.Key))
//		{
//			Elem.Value->SetColorAndOpacity(TurretPortraitsColor[Elem.Key]);
//		}
//		else
//		{
//			FLinearColor NewColor = TurretPortraitsColor[Elem.Key] * 0.4;
//			Elem.Value->SetColorAndOpacity(NewColor);
//		}
//	}
//}