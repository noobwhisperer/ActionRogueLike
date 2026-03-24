// file: SAction.cpp

#include "ActionSystem/SAction.h"

#include "ActionSystem/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	ensureAlways(!bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped Running: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	//TSH: why not just check the flag directly ?
	if(IsRunning())
	{
		return false;
	}

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
