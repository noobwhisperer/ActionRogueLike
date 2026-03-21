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

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(int32 Delta)
{
	if(Health <= 0) return false;

	if (Delta > 0)
	{
		// healing
		if (Health == MaxHealth)
		{
			return false;
		}
	}
	else if (Delta < 0)
	{
		// damage
	}
	else
	{
		// nothing
		return false;
	}

	Health += Delta;
	Health = FMath::Clamp(Health, 0, MaxHealth);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}
