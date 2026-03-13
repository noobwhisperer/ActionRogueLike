// file: SItemChest.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"


class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:

	ASItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	// pitch angle for the lid to open to when the chest is interacted with
	UPROPERTY(EditAnywhere)
	float TargetPitch;

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* LidMesh;


};
