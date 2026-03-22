// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/SCreditsPickup.h"





bool ASCreditsPickup::DoPickup(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

	// USAttributeComponent* AttComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();

	bool bPickedUp = false;

	//TODO: Add checks for Credits and apply the credit pickup to the InstigatorPawn's PlayerState if result does not exceed max credits



	return bPickedUp;
}
