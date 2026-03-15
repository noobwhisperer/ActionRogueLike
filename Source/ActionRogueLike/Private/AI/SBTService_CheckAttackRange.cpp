// file: USBTService_CheckAttackRange.cpp


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI and player, set blackboard value to true if within attack range, false otherwise

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
			AAIController* OwnerController = OwnerComp.GetAIOwner();
			APawn* AIPawn = OwnerController->GetPawn();
			if (ensure(AIPawn) && ensure(OwnerController))
			{
				float DistanceToTarget = FVector::Dist(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
				bool bWithinAttackRange = DistanceToTarget <= 2000.0f; // Example attack range
				bool bHasLOS = false;
				if (bWithinAttackRange)
				{
					bHasLOS = OwnerController->LineOfSightTo(TargetActor);
				}
				BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinAttackRange && bHasLOS));
			}
		}
	}
}
