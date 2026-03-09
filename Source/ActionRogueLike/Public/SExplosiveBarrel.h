// file: SBarrel.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class USRadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:
	ASExplosiveBarrel();

protected:

	/// <summary>
	/// Mesh representing the barrel
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BarrelMeshComp;

	/// <summary>
	/// RadialForce triggered by explosion when hit by projectile
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USRadialForceComponent* ExplosionForceComp;

    UFUNCTION()
    void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                     UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                     const FHitResult& Hit);

public:

	virtual void PostInitializeComponents() override;
};
