//file: SActionComponent.cpp

#include "ActionSystem/SActionComponent.h"

#include "ActionSystem/SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActionTypes)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}


void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		//	TSH :  The design may be getting too complicated here IMO ...
		// The ActionComponent should just be as dumb as possible and only manage the list of actions and their tags.
		// The logic for when to start an action should be in the level blueprint or in the character blueprint or something like that.
		// For example, the statement below presumes that and action that has autostart can never be added to a character that cannot start it
		// immediately. I'm not 100% sure this will always make sense, but maybe I'm overthinking it.

		if(NewAction->bAutoState && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}

	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if(!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	//UE_LOG(LogTemp, Warning, TEXT("   ...StartActionByName : Searching for Action %s"), *ActionName.ToString());

	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			//UE_LOG(LogTemp, Warning, TEXT("   ...StartActionByName : found Action %s"), *Action->ActionName.ToString());
			if (!Action->CanStart(Instigator))
			{
				FString FailMsg = FString::Printf(TEXT("Failed to run %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailMsg);
				//UE_LOG(LogTemp, Warning, TEXT("   ...StartActionByName : failed to start Action ... CanStart test fails %s"), *Action->ActionName.ToString());
				continue;
			}
			//UE_LOG(LogTemp, Warning, TEXT("   ...StartActionByName : Calling Action->StartAction %s"), *Action->ActionName.ToString());
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	//UE_LOG(LogTemp, Warning, TEXT("   ...StopActionByName : Searching for Action %s"), *ActionName.ToString());

	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			//UE_LOG(LogTemp, Warning, TEXT("   ...StopActionByName : found Action %s ... calling Action->StopAction"), *ActionName.ToString());

			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}
