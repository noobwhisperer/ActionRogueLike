// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/SCreditsPickup.h"

#include "SPlayerState.h"


bool ASCreditsPickup::DoPickup(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

    ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
    check(PlayerState);

    bool bCreditsAdjusted = PlayerState->AdjustCredits(CreditAmount);

    UE_LOG(LogTemp, Log, TEXT(" Attempted Credit Pick up ... Player credits = %d"), PlayerState->GetCredits() );

    return bCreditsAdjusted;
}
