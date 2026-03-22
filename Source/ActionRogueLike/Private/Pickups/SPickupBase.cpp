// file: SPickupBase.cpp

#include "Pickups/SPickupBase.h"


// Sets default values
ASPickupBase::ASPickupBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	PrimaryActorTick.bCanEverTick = true;
	PickupState = EPickupState::Available;
	RechargeTime = 10.0f; // Default 10 seconds
}

void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	check(InstigatorPawn);

	// Only allow interaction if the pickup is available
	if (PickupState != EPickupState::Available)
	{
		return;
	}

	if(DoPickup(InstigatorPawn))
	{
		// Pickup was successful, start recharge timer
		PickupState = EPickupState::Recharging;
		BaseMesh->SetScalarParameterValueOnMaterials("IsEmpty", 1.0f);
		GetWorldTimerManager().SetTimer(TimerHandle_Recharge, this, &ASPickupBase::RechargeCompleted, RechargeTime);
	}

	// USAttributeComponent* AttComp = InstigatorPawn->GetComponentByClass<USAttributeComponent>();
	//
	// if (AttComp && AttComp->ApplyHealthChange(this, HealingAmount))
	// {
	// 	// Healing was successful, start recharge timer
	//
	// 	PotionState = EPotionState::Recharging;
	// 	BaseMesh->SetScalarParameterValueOnMaterials("IsEmpty", 1.0f);
	// 	GetWorldTimerManager().SetTimer(TimerHandle_Recharge, this, &ASHealthPotion::RechargeCompleted, RechargeTime);
	// }
}

void ASPickupBase::RechargeCompleted()
{
	PickupState = EPickupState::Available;
	BaseMesh->SetScalarParameterValueOnMaterials("IsEmpty", 0.0f);
}


