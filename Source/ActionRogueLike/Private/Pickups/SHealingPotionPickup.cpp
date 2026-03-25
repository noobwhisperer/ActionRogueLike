// file: SHealingPotionPickup.cpp

#include "Pickups/SHealingPotionPickup.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"


bool ASHealingPotionPickup::DoPickup(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	USAttributeComponent* AttComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();

	check(PlayerState);
	check(AttComp);

	bool bPickupSuccessful = false;

	if(PlayerState->RemoveCredits(CreditCost))
	{
		// we successfully removed the credits, so we can afford the pickup
		bPickupSuccessful = true;
		UE_LOG(LogTemp, Log, TEXT("HealthPotion purchased and applied! Player now has %d credits left"), PlayerState->GetCredits());
		bool bPotionApplied = AttComp->ApplyHealthChange(this, HealingAmount);
		if(!bPotionApplied)
		{
			// couldn't heal (might already be fully healed)
			UE_LOG(LogTemp, Log, TEXT("HealthPotion unable to be applied at this time! We may have wasted our credits."));	
		}
	}
	else
	{
		// couldn't afford the potion
		UE_LOG(LogTemp, Log, TEXT("Not enough credits to purchase HealthPotion!"));
	}

	return bPickupSuccessful;


	// // see if we can afford the pickup
	// int32 CreditsAvailable = PlayerState->GetCredits();
	//
	// if (CreditsAvailable < CreditCost)
	// {
	// 	// we can't afford the potion
	// 	UE_LOG(LogTemp, Log, TEXT("Not enough credits to purchase HealthPotion!"));
	// 	return false;
	// }
	//
	// if (AttComp->ApplyHealthChange(this, HealingAmount))
	// {
	// 	PlayerState->AdjustCredits(-CreditCost);
	//
	// 	// potion purchased and applied
	// 	UE_LOG(LogTemp, Log, TEXT("HealthPotion purchased and applied! Player now has %d credits left"), PlayerState->GetCredits());
	// 	return true;
	// }
	//
	// // couldn't heal (might already be fully healed)
	// UE_LOG(LogTemp, Log, TEXT("HealthPotion unable to be applied at this time!"));
	// return false;
}
