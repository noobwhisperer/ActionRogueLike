// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	// maybe need a nullptr check ???
	if (FirstBT)
	{
		RunBehaviorTree(FirstBT);

		// a bit of a hack I think, but he mentions we'll do a better approach later
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if(PlayerPawn)
		{
			GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
		}
	}
}
