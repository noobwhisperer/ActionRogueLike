//file: SInteractionComponent.cpp


#include "SInteractionComponent.h"

#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"


static TAutoConsoleVariable<bool> CVarDrawInteractionDebug(TEXT("roguelike.DrawInteractionDebug"), false, TEXT("Draw debug lines for interaction component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USInteractionComponent::PrimaryInteract()
{
	PrimaryInteract_SphereSweep();
}


void USInteractionComponent::PrimaryInteract_SphereSweep()
{
	// perform query to determine if we are near an "interactible" object  (and looking at it)

	TArray<FHitResult> OutHitResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape CollisionShape;
	const float CollisionRadius = 30.0f;
	CollisionShape.SetSphere(CollisionRadius);

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);


	GetWorld()->SweepMultiByObjectType(OutHitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);


	const float DebugDrawLifetime = 5.0f;
	const bool bDebugDraw = CVarDrawInteractionDebug.GetValueOnGameThread();
	FColor LineColor = FColor::Red;

	for (FHitResult HitResult : OutHitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		FColor LineColorSphere = FColor::Red;

		if (HitActor && HitActor->Implements<USGameplayInterface>())
		{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			ISGameplayInterface::Execute_Interact(HitActor, OwnerPawn);

			LineColorSphere = FColor::Green;
			LineColor = FColor::Green;
			if (bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, CollisionRadius, 32, LineColorSphere, false, DebugDrawLifetime);
			}

			break; // stop processing the hit list once we find the first interactible object
		}

		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, CollisionRadius, 32, LineColorSphere, false, DebugDrawLifetime);
		}
	}

	if (bDebugDraw)
	{
		// debugging : draw the query line - might need to do this
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, DebugDrawLifetime, 0, DebugDrawLifetime);
	}
}
