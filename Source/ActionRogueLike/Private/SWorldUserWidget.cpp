// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ScreenPosition;

	bool bProjected = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenPosition);
	if (bProjected)
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition = ScreenPosition / Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}


