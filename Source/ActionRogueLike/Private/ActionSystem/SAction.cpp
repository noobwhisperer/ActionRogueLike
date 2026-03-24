// file: SAction.cpp

#include "ActionSystem/SAction.h"

#include "ActionSystem/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	bIsRunning = true;

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped Running: %s"), *GetNameSafe(this));
	bIsRunning = false;


	USActionComponent* Comp = GetOwningComponent();
	check(Comp);

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
