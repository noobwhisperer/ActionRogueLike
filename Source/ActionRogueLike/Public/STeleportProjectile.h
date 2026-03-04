// file: STeleportProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"


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
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()


public:
	ASTeleportProjectile();

	UPROPERTY(EditAnywhere, Category = "Teleport Projectile")
	float MotionTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Teleport Projectile")
	float TeleportTimer = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* TeleportNiagaraComp;

protected:
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_StateTimer;

	EProjectileState ProjectileState = EProjectileState::Unknown;

	void ExplodeProjectile();
	void TeleportInstigator();

public:
	virtual void Tick(float DeltaTime) override;
};
