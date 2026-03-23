// file: SCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USActionComponent;
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectialClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectialClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float TargetDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FColor DamageColor = FColor::Red;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageSpeed = 1.0f;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_DashAttack;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	virtual void PostInitializeComponents() override;


	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, int32 NewHealth, int32 Delta);


	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();


	void PrimaryAttack();
	void BlackHoleAttack();
	void TeleportAttack();

	void PrimaryAttack_TimeElapsed();
	void BlackHoleAttack_TimeElapsed();
	void TeleportAttack_TimeElapsed();

	void PrimaryInteract();

	FTransform ComputeProjectileLaunchTransform() const;

	void SpawnProjectile(TSubclassOf<AActor> ProjectileToSpawn);

	void DebugDrawRotationViz() const;

	virtual FVector GetPawnViewLocation() const override;

public:


	ASCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(int32 Amount = 100);


};
