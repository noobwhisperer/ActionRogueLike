// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 Credits = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 MaxCredits = 0;

public:

	ASPlayerState();


	/// <summary>
	/// Adjust Credits by the given amount. Returns true if the adjustment was successful, false if it would have put
	/// the credits outside of the allowed range (0 to MaxCredits).
	/// </summary>
	/// <param name="Amount">Amount to adjust the credits by.</param>
	/// <returns>True if the adjustment was successful, false otherwise.</returns>
	bool AdjustCredits(int32 Amount)
	{
		int32 NewCredits = Credits + Amount;
		if (NewCredits > MaxCredits || NewCredits < 0)
		{
			return false;
		}
		Credits = NewCredits;
		return true;
	}

	int32 GetCredits() const { return Credits; }
	int32 GetMaxCredits() const { return MaxCredits; }
};
