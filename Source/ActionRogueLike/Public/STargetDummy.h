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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	/// <summary>
	/// Mesh representing the barrel
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, int32 NewHealth, int32 Delta);


};
