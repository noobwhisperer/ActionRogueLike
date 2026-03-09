// file: STeleportProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"


enum class EProjectileState
{
	Unknown = 0
	, InitialMovement
	, WaitingToTeleportInstigator
};


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASDashProjectile();

	virtual void Tick(float DeltaTime) override;// just debugging stuff for now, will remove later



protected:

	/// <summary>
	/// Projectile will detonate after this amount of time has
	/// passed since projectile was spawned. Detonation will cause projectile to stop
	/// moving and start teleport timer. The projectile will also detonate if it hits
	/// something before the timer runs out.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Dash Projectile")
	float DetonationTimer = 0.2f;

	/// <summary>
	/// Teleport instigator after this amount of time has passed.
	/// Teleportation will cause projectile to destroy itself and
	/// teleport instigator to projectile's location. Teleport timer starts after projectile detonates.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Dash Projectile")
	float TeleportTimer = 0.2f;


	// no longer needed ... use the base class Vfx
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// UNiagaraComponent* TeleportNiagaraComp;



	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_StateTimer;

	EProjectileState ProjectileState = EProjectileState::Unknown;


	/// <summary>
	/// override the base explode function to stop movement and start teleport timer instead of destroying the projectile immediately.
	/// We will destroy the projectile after teleporting the instigator.
	/// </summary>
	virtual void Explode_Implementation() override;



	// void ExplodeProjectile();

	void TeleportInstigator();



};
