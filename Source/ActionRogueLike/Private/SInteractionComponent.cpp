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

	// perform query to determine if we are near an "interactable" object  (and looking at it)

	FHitResult OutHitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector()*1000);

	bool hitAGameplayObject = false;

	GetWorld()->LineTraceSingleByObjectType(OutHitResult, EyeLocation, End, ObjectQueryParams);

	AActor* HitActor = OutHitResult.GetActor();

	if(HitActor && HitActor->Implements<USGameplayInterface>() )
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

