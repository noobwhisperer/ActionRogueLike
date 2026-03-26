// file: SMagicProjectile.cpp

#include "SMagicProjectile.h"

#include "SGameplayFunctionLibrary.h"
#include "ActionSystem/SAction_Effect.h"
#include "ActionSystem/SActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	DamageAmount = 20.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			// Parry logic - reflect the projectile

			ProjectileMoveComp->Velocity = -ProjectileMoveComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if(ActionComp)
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}
