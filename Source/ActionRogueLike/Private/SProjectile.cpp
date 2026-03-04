// file: SProjectile.cpp


#include "SProjectile.h"


#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"

ASProjectile::ASProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");

	RootComponent = SphereComp;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComp");
	NiagaraComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();

	// code to ignore collision for the APawn that spawned us
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

