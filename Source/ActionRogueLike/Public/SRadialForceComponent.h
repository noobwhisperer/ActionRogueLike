// File: SRadialForceComponent.h

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SRadialForceComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USRadialForceComponent : public URadialForceComponent
{
	GENERATED_BODY()

public:

	/// <summary>
	/// Amount of damage to apply to any affected SAttributeComponents
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 RadialForceDamage = 20;

	/// <summary>
	/// AActor that is responsible for this radial force. This is used to
	///	reward kill credits to the player when the radial force kills an enemy
	/// </summary>
	UPROPERTY(EditAnywhere)
	AActor* Instigator = nullptr;

	/// <summary>
	/// Override the FireImpulse method to do our own version which applies damage to
	///	any SAttributeComponents that are found
	/// </summary>
	virtual void FireImpulse() override;
	
};
