//file: SAttributeComponent.cpp

#include "SAttributeComponent.h"


USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealth());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, int32 Delta)
{

	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	int32 OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	int32 ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;

}
