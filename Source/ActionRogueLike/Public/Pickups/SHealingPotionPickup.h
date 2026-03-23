// file: SHealingPotionPickup.h

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SHealingPotionPickup.generated.h"

/**
 * A health potion pickup that restores health to the instigator pawn.
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API ASHealingPotionPickup : public ASPickupBase
{
	GENERATED_BODY()
	
protected:

	/// <summary>
	/// Amount of healing for this pickup
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	int32 HealingAmount = 0;

	/// <summary>
	/// price of this pickup in Credits
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	int32 CreditCost = 0;

public:

	virtual bool DoPickup(APawn* InstigatorPawn) override;

};
