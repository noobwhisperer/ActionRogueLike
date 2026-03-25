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

// I'm not 100% clear about why we need to do this.
//note: this is a bit of a hack to get the world context, but it works because the outer of this action is the component,
//and the component is owned by the character, which is in the world. We could also consider passing in the world context
//as a parameter to the StartAction function, but that would require changing the signature of the function and all
//its overrides. Alternatively, we could store a reference to the world in the action when it's created, but that would
//require more code and potential for bugs if we forget to set it. Overall, this seems like a reasonable solution for now.
//
// look into this later ... without it, some nodes are not available in the derived BPs (e.g. Delay).
//

UWorld* USAction::GetWorld() const
{
	//note: this was set in the SActionComponent::AddAction function when the action was created.
	//	The outer of the action is the component that owns it.
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());

	if (ActionComp == nullptr)
	{
		return nullptr;
	}

	UWorld* TheOuterWorld = ActionComp->GetWorld();

	return TheOuterWorld;
}
