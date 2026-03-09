// file: SBarrel.cpp

#include "SExplosiveBarrel.h"

#include "SMagicProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "SRadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	BarrelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = BarrelMeshComp;

	ExplosionForceComp = CreateDefaultSubobject<USRadialForceComponent>(TEXT("RadialForceComp"));
	ExplosionForceComp->SetupAttachment(RootComponent);

	// configure the radial force - defaults
	ExplosionForceComp->Radius = 1000.0f;
	ExplosionForceComp->RadialForceDamage = 20.0f;
	ExplosionForceComp->ImpulseStrength = 200000.0f;
	ExplosionForceComp->bImpulseVelChange = false;
	ExplosionForceComp->bAutoActivate = false;
	ExplosionForceComp->Falloff = ERadialImpulseFalloff::RIF_Linear;
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BarrelMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnBarrelHit);
}

void ASExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	ASMagicProjectile* MagicProjectile = Cast<ASMagicProjectile>(OtherActor);
	if (MagicProjectile)
	{
		ExplosionForceComp->FireImpulse();
	}

	// demonstrate debugging logs
	{
		if (OtherActor && OtherActor != this)
		{
			// Your hit logic here
			UE_LOG(LogTemp, Warning, TEXT("Barrel hit by: %s"), *OtherActor->GetName());
		}
		UE_LOG(LogTemp, Log, TEXT("OnBarrelHit in ASExplosiveBarrel class!"));
		UE_LOG(LogTemp, Warning, TEXT("Other Actor = %s, at game time = %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
		FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactPoint + FVector(0.0f, 0.0f, 150.0f), CombinedString, nullptr, FColor::Orange, 2.0f, true);
	}
}
