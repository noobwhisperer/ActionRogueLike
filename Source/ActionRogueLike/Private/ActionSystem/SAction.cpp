// file: SAction.cpp

#include "ActionSystem/SAction.h"

#include "ActionSystem/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("StartAction_Implementation : Start Running: %s"), *GetNameSafe(this));

	ensureAlways(!bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("StopAction_Implementation : Stop Running: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	//TSH: why not just check the flag directly ?
	if (IsRunning())
	{
		//UE_LOG(LogTemp, Log, TEXT("CanStart_Implementation : Already Running !! returning FALSE: %s"), *GetNameSafe(this));

		return false;
	}

	USActionComponent* Comp = GetOwningComponent();
	check(Comp);

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		//UE_LOG(LogTemp, Log, TEXT("CanStart_Implementation : BLOCKED TAGS !! returning FALSE: %s"), *GetNameSafe(this));
		return false;
	}

	//UE_LOG(LogTemp, Log, TEXT("CanStart_Implementation : Can Start ...  returning TRUE: %s"), *GetNameSafe(this));
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
