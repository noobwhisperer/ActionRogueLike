//file: SAttributeComponent.cpp

#include "SAttributeComponent.h"


USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{

	Health += Delta;

	Health = FMath::Clamp(Health, 0, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);


	return true;// extend to check if apply was executed
}
