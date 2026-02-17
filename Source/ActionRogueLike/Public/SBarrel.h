// file: SBarrel.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBarrel : public AActor
{
	GENERATED_BODY()
	
public:
	ASBarrel();

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
	URadialForceComponent* ExplosionForceComp;

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                     UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                     const FHitResult& Hit);

public:

	virtual void PostInitializeComponents() override;


	virtual void Tick(float DeltaTime) override;
};
