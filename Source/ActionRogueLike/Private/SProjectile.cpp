// file: SProjectile.cpp


#include "SProjectile.h"


#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "SAttributeComponent.h"

ASProjectile::ASProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnActorOverlap);


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

void ASProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));

		if( AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);

			Destroy();	// Hmmm - I'm not sure this is a good idea, but I'll see what he shows us next
		}
	}
}
