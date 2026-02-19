//file: SInteractionComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


/// <summary>
/// responsible for handling a character's interactions with objects in the scene (e.g. a treasure chest)
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:


	void PrimaryInteract();
	USInteractionComponent();

protected:

	virtual void BeginPlay() override;

	void PrimaryInteract_Line();
	void PrimaryInteract_SphereSweep();


public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
