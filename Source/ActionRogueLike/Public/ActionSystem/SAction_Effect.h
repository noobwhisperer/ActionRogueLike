// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/SAction.h"
#include "SAction_Effect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Effect : public USAction
{
	GENERATED_BODY()

public:

	USAction_Effect();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period = 0.f;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;
};
