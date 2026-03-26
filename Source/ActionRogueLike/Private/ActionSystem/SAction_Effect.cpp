// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SAction_Effect.h"

#include "ActionSystem/SActionComponent.h"


USAction_Effect::USAction_Effect()
{
	bAutoState = true;
}

void USAction_Effect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USAction_Effect::StopAction_Implementation(AActor* Instigator)
{

	// if the period is still active, execute the effect one last time before stopping if
	// it was just about to execute.
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	// remove this action from the action component so it can be destroyed
	USActionComponent* Comp = GetOwningComponent();
	if(Comp)
	{
		Comp->RemoveAction(this);
	}

}

void USAction_Effect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
}
