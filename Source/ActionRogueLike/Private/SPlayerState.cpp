// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"


ASPlayerState::ASPlayerState()
{
}

bool ASPlayerState::AdjustCredits(int32 Amount)
{
	int32 NewCredits = Credits + Amount;
	if (NewCredits > MaxCredits || NewCredits < 0)
	{
		return false;
	}
	Credits = NewCredits;
	OnCreditsChanged.Broadcast(this, Credits);
	return true;
}
