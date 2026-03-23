// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/SAction_ProjectileAttack.h"

#include "NiagaraFunctionLibrary.h"
#include "SCharacter.h"
#include "ActionSystem/SActionComponent.h"
#include "Kismet/GameplayStatics.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//note: I think we should use ASCharacter herre instead of ACharacter.

	ASCharacter* InstigatorCharacter = Cast<ASCharacter>(Instigator);

	if (InstigatorCharacter == nullptr)
	{
		return;
	}

	InstigatorCharacter->PlayAnimMontage(AttackAnimMontage);


	// spawn the casting effect on the hand socket

	if (CastingEffect)
	{
		//UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName);
		UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, InstigatorCharacter->GetMesh(), HandSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}

	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "AttackDelay_Elapsed", InstigatorCharacter);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate, AttackAnimDelay, false);
}

void USAction_ProjectileAttack::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
}


// I'm not 100% clear about why we need to do this.
//note: this is a bit of a hack to get the world context, but it works because the outer of this action is the component,
//and the component is owned by the character, which is in the world. We could also consider passing in the world context
//as a parameter to the StartAction function, but that would require changing the signature of the function and all
//its overrides. Alternatively, we could store a reference to the world in the action when it's created, but that would
//require more code and potential for bugs if we forget to set it. Overall, this seems like a reasonable solution for now.
UWorld* USAction_ProjectileAttack::GetWorld() const
{
	//note: this was set in the SActionComponent::AddAction function when the action was created.
	//	The outer of the action is the component that owns it.
	USActionComponent* ActionComp = Cast<USActionComponent>(GetOuter());

	if (ActionComp == nullptr)
	{
		return nullptr;
	}

	UWorld* TheOuterWorld = ActionComp->GetWorld();

	return TheOuterWorld;
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!ensure(ProjectileClass))
	{
		StopAction(InstigatorCharacter);
		return;
	}

	FTransform SpawnTM = ComputeProjectileLaunchTransform(InstigatorCharacter);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;


	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	StopAction(InstigatorCharacter);
}


FTransform USAction_ProjectileAttack::ComputeProjectileLaunchTransform(ACharacter* InstigatorCharacter) const
{
	FHitResult OutHitResult;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorCharacter);

	FVector CameraLocation = InstigatorCharacter->GetPawnViewLocation();
	FVector CameraDirection = InstigatorCharacter->GetControlRotation().Vector();


	FVector TargetLocation = CameraLocation + TargetingDistance * CameraDirection;

	GetWorld()->LineTraceSingleByObjectType(OutHitResult, CameraLocation, TargetLocation, ObjectQueryParams, QueryParams);

	if (OutHitResult.bBlockingHit)
	{
		TargetLocation = OutHitResult.ImpactPoint;
	}

	FVector RightHandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

	FVector ProjectileForward = TargetLocation - RightHandLocation;
	ProjectileForward.Normalize();

	FRotator ProjectileRotation = FRotationMatrix::MakeFromXZ(ProjectileForward, FVector(0.0f, 0.0f, 1.0f)).Rotator();


	//temp hack - debugging
	//RightHandLocation.Z = RightHandLocation.Z + 100.0;

	FTransform SpawnTM = FTransform(ProjectileRotation, RightHandLocation);


	// debugging : draw the query line and the projectile trajectory ... reveals "parallax"
	//
	// const float Lifetime = 2.0f;
	// FColor LineColor = OutHitResult.bBlockingHit ? FColor::Green : FColor::Red;
	// DrawDebugLine(GetWorld(), (CameraLocation + 20.0f * CameraDirection), TargetLocation, LineColor, false, Lifetime, 0, 0.0f);
	// DrawDebugLine(GetWorld(), RightHandLocation, TargetLocation, FColor::Blue, false, Lifetime, 0, 0.0f);

	return SpawnTM;
}
