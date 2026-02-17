// file: SMagicProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
// class UParticleSystemComponent;
class UNiagaraComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ASMagicProjectile();

protected:



	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	// UPROPERTY(VisibleAnywhere)
	// UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComp;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	
	
};
