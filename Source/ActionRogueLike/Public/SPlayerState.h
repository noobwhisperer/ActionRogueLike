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

public:
	ASPlayerState();


	/// <summary>
	/// Adds or removes credits from the player, but never allows the amount to go below 0.
	///
	///	OnCreditsChanged will only be broadcast if the credits were adjusted.
	///
	/// </summary>
	/// <param name="Amount">The amount to adjust. Positive values result in adding
	/// credits, negative values result in removing credits.</param>
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AdjustCredits(int32 Amount);

	/// <summary>
	/// Removes credits from the player if they have enough, and returns true.
	/// If they don't have enough credits, returns false and doesn't adjust the amount.
	///	If Amount is negative, returns false and doesn't adjust the amount.
	///	If Amount is zero, returns true and doesn't adjust the amount.
	///
	///	OnCreditsChanged will only be broadcast if the credits were adjusted.
	///
	/// </summary>
	/// <param name="Amount">The amount to remove. Must be positive or zero</param>
	/// <returns>true if the credits were successfully removed, false otherwise</returns>
	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Credits")
	int32 GetCredits() const { return Credits; }


	/// <summary>
	/// Assignable delegate called any time the Credits amount changes
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

};
