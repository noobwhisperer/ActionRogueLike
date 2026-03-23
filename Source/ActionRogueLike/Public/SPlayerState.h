// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"



//TSH - maybe I don't need to pass the player state ... I'm passing the NewCredits amount
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits);

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 Credits = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 MaxCredits = 0;


	/// <summary>
	/// Assignable delegate called any time the Credits amount changes
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

public:
	ASPlayerState();

	/// <summary>
	/// Adjust Credits by the given amount. Returns true if the adjustment was successful, false if it would have put
	/// the credits outside of the allowed range (0 to MaxCredits).
	/// </summary>
	/// <param name="Amount">Amount to adjust the credits by.</param>
	/// <returns>True if the adjustment was successful, false otherwise.</returns>
	bool AdjustCredits(int32 Amount);


	UFUNCTION(BlueprintPure, Category = "Player")
	int32 GetCredits() const { return Credits; }


	UFUNCTION(BlueprintPure, Category = "Player")
	int32 GetMaxCredits() const { return MaxCredits; }
};
