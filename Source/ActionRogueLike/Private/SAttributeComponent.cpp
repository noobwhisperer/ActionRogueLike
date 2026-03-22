//file: SAttributeComponent.cpp

#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("roguelike.DamageMultiplier"), 1.0f, TEXT("Global multiplier for damage taken."), ECVF_Cheat);

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
	if (!GetOwner()->CanBeDamaged() && Delta < 0)
	{
		return false;
	}

	if (Delta < 0)
	{
		Delta = FMath::RoundToInt(Delta * CVarDamageMultiplier.GetValueOnGameThread()); // apply damage multiplier from console variable
	}

	int32 OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);

	int32 ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);


	//check for death
	//note: this is a bit of a code smell, but for now we can use this to notify the
	//gamemode of kills and deaths, which will be used for spawn logic and eventually
	//kill credit and score tracking (server auth etc)

	if (ActualDelta < 0 && Health == 0)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}


	return ActualDelta != 0;
}
