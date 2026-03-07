//file: STargetDummy.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"



class UStaticMeshComponent;

class USAttributeComponent;


UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTargetDummy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	/// <summary>
	/// Mesh representing the barrel
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);


};
