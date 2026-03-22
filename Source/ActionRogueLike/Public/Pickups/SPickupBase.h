// file: SPickupBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPickupBase.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EPickupState : uint8
{
	Available
	, Recharging
};

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASPickupBase();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	EPickupState PickupState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	float RechargeTime;

	FTimerHandle TimerHandle_Recharge;

	void RechargeCompleted();

	/// <summary>
	/// Derived classes must override this function to implement their own pickup logic.
	/// </summary>
	/// <param name="InstigatorPawn">The pawn that is attempting to interact with the pickup.</param>
	/// <returns>true if a pickup occurred</returns>
	virtual bool DoPickup(APawn* InstigatorPawn)
	{
		unimplemented();
		return false;
	}
};
