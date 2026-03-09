//file: SAttributeComponent.cpp

#include "SAttributeComponent.h"


USAttributeComponent::USAttributeComponent()
{
	Health = 100;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{

	Health += Delta;


	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);


	return true;// extend to check if apply was executed
}
