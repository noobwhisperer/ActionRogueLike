// file: SProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ASProjectileBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* EffectComp;

	UPROPERTY(EditAnywhere, Category = "SProjectile")
	UNiagaraSystem* ImpactVfx;

	UPROPERTY(EditAnywhere, Category = "SProjectile")
	bool bShowDebugGraphics = false;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

};
