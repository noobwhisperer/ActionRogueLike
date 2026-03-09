// file: SProjectile.cpp


#include "SProjectileBase.h"


#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// not used yet ... but we'll see what Tom does with this in the next video (he currently doesn't deal damage here)
//#include "SAttributeComponent.h"

ASProjectileBase::ASProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UNiagaraComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
	ProjectileMoveComp->bInitialVelocityInLocalSpace = true;
	ProjectileMoveComp->bRotationFollowsVelocity = true;
	ProjectileMoveComp->ProjectileGravityScale = 0.0f;
	ProjectileMoveComp->InitialSpeed = 8000.0f;
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// code to ignore collision for the APawn that spawned us
	//TSH - synching up to Tom's version ... he has this commented out for some reason ?
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}


void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


// _Implementation from it being marked as BlueprintNativeEvent
void ASProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(!IsActorBeingDestroyed()))
	{
		// UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		// need to use the UE5 Niagara version of this, which is a little different
		if (ImpactVfx)
		{
			// Spawn the Niagara effect at the projectile's location and rotation
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());
		}

		Destroy();
	}
}


//TSH - synching up to Tom's version ... He uses Hit instead of overlap , and doesn't deal damage here
//
// void ASProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (OtherActor)
// 	{
// 		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
//
// 		if( AttributeComp)
// 		{
// 			AttributeComp->ApplyHealthChange(-20.0f);
//
// 			Destroy();	// Hmmm - I'm not sure this is a good idea, but I'll see what he shows us next
// 		}
// 	}
// }
