// file: SMagicProjectile.cpp


#include "SMagicProjectile.h"

#include "SGameplayFunctionLibrary.h"
#include "ActionSystem/SActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	DamageAmount = 20.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			// Handle parry logic here - reflect the projectile,

			ProjectileMoveComp->Velocity = -ProjectileMoveComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));


			//TSH - TODO: might need to push it back in the reflected direction a bit to prevent it from immediately hitting the actor
			// again on the next tick? We could do this by adding a small offset to the projectile's location in the direction of the new velocity.
			// But maybe not necessary since we set ourselves as Instigator



			return;
		}



		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}
