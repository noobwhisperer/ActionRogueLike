//file: SInteractionComponent.cpp


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"


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
	// choose which one to use at compile time for now. Just keeping both styles for reference

	//PrimaryInteract_Line();
	PrimaryInteract_SphereSweep();
}


void USInteractionComponent::PrimaryInteract_Line()
{
	// perform query to determine if we are near an "interactable" object  (and looking at it)

	FHitResult OutHitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	bool hitAGameplayObject = false;

	GetWorld()->LineTraceSingleByObjectType(OutHitResult, EyeLocation, End, ObjectQueryParams);

	AActor* HitActor = OutHitResult.GetActor();

	if (HitActor && HitActor->Implements<USGameplayInterface>())
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		ISGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
		hitAGameplayObject = true;
	}


	// debugging : draw the query line - might need to do this
	const float Lifetime = 2.0f;
	FColor LineColor = hitAGameplayObject ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, Lifetime, 0, 2.0f);
}

void USInteractionComponent::PrimaryInteract_SphereSweep()
{
	// perform query to determine if we are near an "interactable" object  (and looking at it)

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


	bool bHitAGameplayObject = false;
	const float DebugDrawLifetime = 10.0f;


	for (FHitResult HitResult : OutHitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		FColor LineColorSphere = FColor::Red;
		if (HitActor && HitActor->Implements<USGameplayInterface>())
		{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			ISGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
			bHitAGameplayObject = true;
			LineColorSphere = FColor::Green;

			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, CollisionRadius, 32, LineColorSphere, false, 2.0f);

			break;	// stop processing the hit list once we find the first interactable object
		}

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, CollisionRadius, 32, LineColorSphere, false, DebugDrawLifetime);
	}

	// debugging : draw the query line - might need to do this
	FColor LineColor = bHitAGameplayObject ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, DebugDrawLifetime, 0, 2.0f);


}
