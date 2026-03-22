// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickupBase.h"
#include "SCreditsPickup.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API ASCreditsPickup : public ASPickupBase
{
	GENERATED_BODY()
	

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	int32 CreditAmount;

public:

	virtual bool DoPickup(APawn* InstigatorPawn) override;	
	
	
};
