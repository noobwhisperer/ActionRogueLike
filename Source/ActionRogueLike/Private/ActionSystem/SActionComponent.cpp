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
		AddAction(ActionClass);
	}
}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}


void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
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
