// file: SHeatlhPotionPickup.cpp

#include "Pickups/SHeatlhPotionPickup.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"


bool ASHeatlhPotionPickup::DoPickup(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	USAttributeComponent* AttComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();

	check(PlayerState);
	check(AttComp);

	// see if we can afford the pickup

	int32 CreditsAvailable = PlayerState->GetCredits();

	if (CreditsAvailable < CreditCost)
	{
		// we can't afford the potion
		UE_LOG(LogTemp, Log, TEXT("Not enough credits to purchase HealthPotion!"));
		return false;
	}

	if (AttComp->ApplyHealthChange(this, HealingAmount))
	{
		bool bCreditsCharged = PlayerState->AdjustCredits(-CreditCost);
		check(bCreditsCharged)

		// potion purchased and applied
		UE_LOG(LogTemp, Log, TEXT("HealthPotion purchased and applied!"));
		return true;
	}

	// couldn't heal (might already be fully healed)
	UE_LOG(LogTemp, Log, TEXT("HealthPotion unable to be applied at this time!"));
	return false;
}
