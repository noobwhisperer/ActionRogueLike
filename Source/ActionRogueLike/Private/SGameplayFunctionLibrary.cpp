// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"


static TAutoConsoleVariable<float> CVarDeathImpulseMultiplier(TEXT("roguelike.DeathImpulseMultiplier"), 2000.0f, TEXT("Global multiplier for damage taken."), ECVF_Cheat);


#pragma optimize("",off)

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

		//debugging - note: still might be a bit of a nasty window where it's possible to hit this ... e.g. player kills bot at same moment play dies?
		//	might be better to clean this up and send in the "Shot Direction" computed at projectile spawn time instead of trying to
		//	figure it out by referencing through a bunch of pointers to things that may not exist.
		//
		//
		// if(InstigatorActor == nullptr)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Unlucky!"));
		// }

		if(HitComp && InstigatorActor && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector ShotDirection = HitResult.ImpactPoint - InstigatorActor->GetActorLocation();
			ShotDirection.Normalize();

			// add a little vertical "lift"
			ShotDirection.Z = 1.5f;
			ShotDirection.Normalize();

			const float DeathImpulseMultiplier = CVarDeathImpulseMultiplier.GetValueOnGameThread();
			HitComp->AddImpulseAtLocation(ShotDirection * DamageAmount * DeathImpulseMultiplier, HitResult.ImpactPoint);
		}
		return true;
	}

	return false;
}


#pragma optimize("",on)
