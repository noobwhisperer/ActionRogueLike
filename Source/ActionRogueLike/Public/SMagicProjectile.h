// file: SMagicProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	

	ASMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
		
	UPROPERTY(EditDefaultsOnly, Category = "SMagicProjectile")
	float DamageAmount;


	/// <summary>
	/// This projectile will respond to parry attempts if the "other actor" matches this assigned ParryTag
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "SMagicProjectile")
	FGameplayTag ParryTag;


};
