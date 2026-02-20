// file: SBarrel.cpp

#include "SBarrel.h"

#include "SMagicProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBarrel::ASBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	BarrelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    RootComponent = BarrelMeshComp;

    
    ExplosionForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    ExplosionForceComp->SetupAttachment(RootComponent);

    // configure the radial force - defaults
    ExplosionForceComp->Radius = 1000.0f;
    ExplosionForceComp->ImpulseStrength = 200000.0f;
    ExplosionForceComp->bImpulseVelChange = false;
    ExplosionForceComp->bAutoActivate = false;
    ExplosionForceComp->Falloff = ERadialImpulseFalloff::RIF_Linear;
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



    // demonstrate debugging logs

    UE_LOG(LogTemp, Log, TEXT("OnBarrelHit in ASBarrel class!"));

    UE_LOG(LogTemp, Warning, TEXT("Other Actor = %s, at game time = %f"), *GetNameSafe(OtherActor) , GetWorld()->TimeSeconds);

    FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
    DrawDebugString(GetWorld(), Hit.ImpactPoint + FVector(0.0f, 0.0f, 150.0f), CombinedString, nullptr, FColor::Orange, 2.0f, true);

}