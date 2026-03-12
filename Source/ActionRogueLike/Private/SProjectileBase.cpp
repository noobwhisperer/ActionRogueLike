// file: SProjectile.cpp

#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ASProjectileBase::ASProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UNiagaraComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->bAutoActivate = false;

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

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(ProjectileSoundCue && AudioComp)
	{
		AudioComp->SetSound(ProjectileSoundCue);
		AudioComp->Play();
	}
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

		if(ImpactSoundCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSoundCue, GetActorLocation());
		}

		Destroy();
	}
}

void ASProjectileBase::Destroyed()
{
	// Stop the projectile sound if it's still playing
	if(AudioComp && AudioComp->IsPlaying())
	{
		AudioComp->Stop();
	}

	Super::Destroyed();
}
