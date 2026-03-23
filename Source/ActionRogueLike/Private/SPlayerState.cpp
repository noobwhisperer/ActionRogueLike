// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"


ASPlayerState::ASPlayerState()
{
}

bool ASPlayerState::AdjustCredits(int32 Amount)
{
	int32 NewCredits = Credits + Amount;
	if (Credits == MaxCredits || NewCredits < 0)
	{
		return false;
	}
	Credits = FMath::Clamp(NewCredits, 0, MaxCredits);
	OnCreditsChanged.Broadcast(this, Credits);
	return true;
}
