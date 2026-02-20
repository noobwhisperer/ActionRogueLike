// file: SGameplayInterface.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SGameplayInterface.generated.h"

UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Responsible for managing interactions between the object that implements this interface and a
 * APawn in the world that is requesting an interaction. (through the BPE "Interact". The Interact BPE
 * is programmable and override is possible with derived BP
 */
class ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

public:

	//note: BlueprintNativeEvent - allows you to define a C++ function AND also override it with BP if you choose


	/// <summary>
	/// Blueprint event for "Interact" that can be defined in C++ AND also override with BP if desired.
	/// </summary>
	/// <param name="InstigatorPawn">The pawn that would like to perform the interaction</param>
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

};
