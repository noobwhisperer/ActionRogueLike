// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"


static TAutoConsoleVariable<float> CVarDeathImpulseMultiplier(TEXT("roguelike.DeathImpulseMultiplier"), 2000.0f, TEXT("Global multiplier for damage taken."), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarDeathImpulseZ(TEXT("roguelike.DeathImpulseZ"), 2.0f, TEXT("Additional Z-component added to death impulse direction."), ECVF_Cheat);

bool USGameplayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* TargetActor, int32 DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if(AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(InstigatorActor, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* InstigatorActor, AActor* TargetActor, int32 DamageAmount, const FHitResult& HitResult)
{
	if(ApplyDamage(InstigatorActor, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();

		if(HitComp && InstigatorActor && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector ShotDirection = HitResult.TraceEnd - HitResult.TraceStart;
			ShotDirection.Normalize();

			const float DeathImpulseMultiplier = CVarDeathImpulseMultiplier.GetValueOnGameThread();
			const float DeathImpulseZ = CVarDeathImpulseZ.GetValueOnGameThread();


			// add a little vertical "lift"
			ShotDirection.Z = DeathImpulseZ;
			ShotDirection.Normalize();

			HitComp->AddImpulseAtLocation(ShotDirection * DamageAmount * DeathImpulseMultiplier, HitResult.ImpactPoint);
		}
		return true;
	}

	return false;
}
