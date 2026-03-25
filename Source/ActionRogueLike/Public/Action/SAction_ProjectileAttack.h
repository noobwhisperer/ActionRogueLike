// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UNiagaraSystem;
class UAnimMontage;
class UWorld;


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:

	USAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocketName = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Attack")
	UNiagaraSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float TargetingDistance = 3000.0f;

	// FTimerHandle TimerHandle_AttackDelay;


	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	FTransform ComputeProjectileLaunchTransform(ACharacter* InstigatorCharacter) const;

};
