// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ToastStackWidget.h"
#include "UI/SummonNotificationWidget.h"
#include "Components/VerticalBox.h"

void UToastStackWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UToastStackWidget::PushToast(const FSummonData& TurretToSummon)
{
    if (!VB_Toasts || !ToastClass) return;

    if (GetVisibility() == ESlateVisibility::Collapsed || GetVisibility() == ESlateVisibility::Hidden)
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    const int32 Count = VB_Toasts->GetChildrenCount();

    for (int32 i = 0; i < Count; ++i)
    {
        if (auto* T = Cast<USummonNotificationWidget>(VB_Toasts->GetChildAt(i)))
        {
            if (!T->IsClosing())
                T->ForceClose(true);
        }
        else
        {
            VB_Toasts->RemoveChildAt(i);
        }
    }

    USummonNotificationWidget* Toast = CreateWidget<USummonNotificationWidget>(GetWorld(), ToastClass);
    if (!Toast) return;

    VB_Toasts->AddChildToVerticalBox(Toast);          
    Toast->DisplaySeconds = FMath::Max(0.01f, Toast->DisplaySeconds);
    Toast->Setup(TurretToSummon);
    Toast->PlayIn();                           

    Toast->OnNotifyFinished.Clear();
    Toast->OnNotifyFinished.AddLambda([this, Toast]()
        {
            if (Toast && Toast->GetParent())
                Toast->RemoveFromParent();

            if (VB_Toasts && VB_Toasts->GetChildrenCount() == 0)
                OnBecameEmpty.Broadcast();
        });

    UE_LOG(LogTemp, Warning, TEXT("PushToast: children=%d"), VB_Toasts->GetChildrenCount());
}

void UToastStackWidget::ClearAll(bool bFastOut)
{
	if (!VB_Toasts) return;

	for (int32 i = VB_Toasts->GetChildrenCount() - 1; i >= 0; --i)
	{
		if (auto* T = Cast<USummonNotificationWidget>(VB_Toasts->GetChildAt(i)))
		{
			T->ForceClose(bFastOut);
		}
		else
		{
			VB_Toasts->RemoveChildAt(i);
		}
	}
}
