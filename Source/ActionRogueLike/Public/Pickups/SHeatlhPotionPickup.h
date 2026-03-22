// file: SHeatlhPotionPickup.h

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SHeatlhPotionPickup.generated.h"

/**
 * A health potion pickup that restores health to the instigator pawn.
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API ASHeatlhPotionPickup : public ASPickupBase
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	int32 HealingAmount;

public:

	virtual bool DoPickup(APawn* InstigatorPawn) override;

};
