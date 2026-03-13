//file: SAttributeComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

//class USAttributeComponent;	// hmmm - Do I need to forward declare this ... looks like he needs to also do this, but somehow his compiled and so does mine

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes" )
	int32 Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes" )
	int32 MaxHealth;

public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	FORCEINLINE int32 GetHealth() const { return Health; }

	FORCEINLINE int32 GetMaxHealth() const { return MaxHealth; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(int32 Delta);

};
