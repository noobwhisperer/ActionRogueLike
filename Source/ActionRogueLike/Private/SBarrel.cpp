// file: SBarrel.cpp

#include "SBarrel.h"

#include "SMagicProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBarrel::ASBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	BarrelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    RootComponent = BarrelMeshComp;

    
    ExplosionForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    ExplosionForceComp->SetupAttachment(RootComponent);

    // configure the radial force - defaults
    ExplosionForceComp->Radius = 500.0f;
    ExplosionForceComp->ImpulseStrength = 2000.0f;
    ExplosionForceComp->bImpulseVelChange = false;
    ExplosionForceComp->bAutoActivate = false; 
}

void ASBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	BarrelMeshComp->OnComponentHit.AddDynamic(this, &ASBarrel::OnBarrelHit);
}

void ASBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                            const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // Your hit logic here
        UE_LOG(LogTemp, Warning, TEXT("Barrel hit by: %s"), *OtherActor->GetName());
    }

   ASMagicProjectile* MagicProjectile = Cast<ASMagicProjectile>(OtherActor);
    if (MagicProjectile)
    {
        ExplosionForceComp->FireImpulse();
    }

    //TODO: A more complete approach would switch the barrel's state to "exploded" and it would not be allowed to explode
    //  again, and the mesh/material might be swapped to show this barrel already blew up

}