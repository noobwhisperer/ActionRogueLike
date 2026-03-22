// file: SHeatlhPotionPickup.cpp

#include "Pickups/SHeatlhPotionPickup.h"

#include "SAttributeComponent.h"


bool ASHeatlhPotionPickup::DoPickup(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

	USAttributeComponent* AttComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();

	bool bPickedUp = false;


	//TODO: Add checks for Credits and charge the InstigatorPawn if they have enough credits to use the potion

	if (AttComp->ApplyHealthChange(this, HealingAmount))
	{
		bPickedUp = true;
	}


	return bPickedUp;
}
