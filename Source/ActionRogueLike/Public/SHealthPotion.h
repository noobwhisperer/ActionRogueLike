// file: SHealthPotion.h

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EPotionState : uint8
{
	Available,
	Recharging
};

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASHealthPotion();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere)
	int32 HealingAmount;

protected:
								
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion")
	EPotionState PotionState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	float RechargeTime;

	FTimerHandle TimerHandle_Recharge;

	void RechargeCompleted();

};
	