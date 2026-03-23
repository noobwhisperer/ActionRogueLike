// file: SAction.cpp

#include "ActionSystem/SAction.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	bIsRunning = true;

}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped Running: %s"), *GetNameSafe(this));
	bIsRunning = false;

}
