// File: SRadialForceComponent.cpp


#include "SRadialForceComponent.h"

#include "DestructibleInterface.h"
#include "SAttributeComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/MovementComponent.h"

void USRadialForceComponent::FireImpulse()
{
	//dont call super - we re-implement the whole thing here
	//Super::FireImpulse();

	const FVector Origin = GetComponentLocation();

	// Find objects within the sphere
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(FireImpulseOverlap), false);

	// Ignore owner actor if desired
	if (bIgnoreOwningActor)
	{
		Params.AddIgnoredActor(GetOwner());
	}

	GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere(Radius), Params);

	// A component can have multiple physics presences (e.g. destructible mesh components).
	// The component should handle the radial force for all of the physics objects it contains
	// so here we grab all of the unique components to avoid applying impulses more than once.
	TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
	AffectedComponents.Reserve(Overlaps.Num());

	TArray<USAttributeComponent*, TInlineAllocator<1>> AffectedAttributeComponents;
	AffectedAttributeComponents.Reserve(Overlaps.Num());

	for (FOverlapResult& OverlapResult : Overlaps)
	{
		if (UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
		{
			AffectedComponents.AddUnique(PrimitiveComponent);

			AActor* OwnerActor = PrimitiveComponent->GetOwner();

			if (OwnerActor)
			{
				if(USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OwnerActor->GetComponentByClass(USAttributeComponent::StaticClass())))
				{
					AffectedAttributeComponents.AddUnique(AttributeComp);
				}
			}
		}
	}

	for (USAttributeComponent* AttributeComp : AffectedAttributeComponents)
	{
		AttributeComp->ApplyHealthChange(Instigator, -RadialForceDamage);
	}

	for (UPrimitiveComponent* PrimitiveComponent : AffectedComponents)
	{
		if (DestructibleDamage > UE_SMALL_NUMBER)
		{
			if (IDestructibleInterface* DestructibleInstance = Cast<IDestructibleInterface>(PrimitiveComponent))
			{
				DestructibleInstance->ApplyRadiusDamage(DestructibleDamage, Origin, Radius, ImpulseStrength, Falloff == RIF_Constant);
			}
		}

		// Apply impulse
		PrimitiveComponent->AddRadialImpulse(Origin, Radius, ImpulseStrength, Falloff, bImpulseVelChange);

		// See if this is a target for a movement component, if so apply the impulse to it
		if (PrimitiveComponent->bIgnoreRadialImpulse == false)
		{
			TInlineComponentArray<UMovementComponent*> MovementComponents;
			if (AActor* OwningActor = PrimitiveComponent->GetOwner())
			{
				OwningActor->GetComponents(MovementComponents);
				for (const auto& MovementComponent : MovementComponents)
				{
					if (MovementComponent->UpdatedComponent == PrimitiveComponent)
					{
						MovementComponent->AddRadialImpulse(Origin, Radius, ImpulseStrength, Falloff, bImpulseVelChange);
						break;
					}
				}
			}
		}
	}
}
