// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"


ASPlayerState::ASPlayerState()
{
}

void ASPlayerState::AdjustCredits(int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}
	Credits = FMath::Max(Credits + Amount, 0);
	OnCreditsChanged.Broadcast(this, Credits);
}

bool ASPlayerState::RemoveCredits(int32 Amount)
{
	if (Amount < 0)
	{
		return false;
	}
	if(Amount == 0)
	{
		return true;
	}
	if (Credits < Amount)
	{
		return false;
	}
	Credits -= Amount;
	OnCreditsChanged.Broadcast(this, Credits);
	return true;
}
