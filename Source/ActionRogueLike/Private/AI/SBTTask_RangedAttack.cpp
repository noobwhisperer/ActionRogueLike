// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return Super::ExecuteTask(OwnerComp, NodeMemory);


	// spawn a projectile and shoot it towards the player


	//TODO: need to check that ProjectileClass is set and valid before trying to spawn it ??

	AAIController* MyController = OwnerComp.GetAIOwner();


	if (!ensure(ProjectileClass))
	{
		return EBTNodeResult::Failed;
	}

	if (!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

	if (!ensure(MyPawn))
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//TSH : note ... feels like these portions below should be a function on
	//	the AICharacter itself ... but for now, we'll just do it here

	FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
