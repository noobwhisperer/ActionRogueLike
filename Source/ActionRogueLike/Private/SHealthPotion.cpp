// file: SHealthPotion.cpp


#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	PrimaryActorTick.bCanEverTick = true;
	PotionState = EPotionState::Available;
	RechargeTime = 10.0f; // Default 10 seconds
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

	// Only allow interaction if the potion is available
	if (PotionState != EPotionState::Available)
	{
		return;
	}

	USAttributeComponent* AttComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();

	if (AttComp && AttComp->ApplyHealthChange(this, HealingAmount))
	{
		// Healing was successful, start recharge timer

		PotionState = EPotionState::Recharging;
		BaseMesh->SetScalarParameterValueOnMaterials("IsEmpty", 1.0f);
		GetWorldTimerManager().SetTimer(TimerHandle_Recharge, this, &ASHealthPotion::RechargeCompleted, RechargeTime);
	}
}

void ASHealthPotion::RechargeCompleted()
{
	PotionState = EPotionState::Available;
	BaseMesh->SetScalarParameterValueOnMaterials("IsEmpty", 0.0f);
}
