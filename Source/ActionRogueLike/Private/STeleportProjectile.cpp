// file: STeleportProjectile.cpp


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


ASTeleportProjectile::ASTeleportProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionTimer = 0.2f;
	TeleportTimer = 0.2f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();


	ProjectileState = EProjectileState::BeginPlay;
}

void ASTeleportProjectile::ExplodeProjectile()
{
	ProjectileState = EProjectileState::WaitingToTeleportInstigator;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASTeleportProjectile::TeleportInstigator, TeleportTimer);

	// set movement velocity to "zero"
	MovementComp->Velocity *= 0.0;

	//TODO: trigger explosion VFX
}

void ASTeleportProjectile::TeleportInstigator()
{
	//TODO: Teleport Instigator


	//destroy ourselves

	Destroy();
}

void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FColor LineColorSphere = FColor::White;

	switch (ProjectileState)
	{
		case EProjectileState::Unknown:
		{
			break;
		}
		case EProjectileState::BeginPlay:
		{
			ProjectileState = EProjectileState::InitialMovement;
			GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASTeleportProjectile::ExplodeProjectile, MotionTimer);
			LineColorSphere = FColor::Red;
			break;
		}
		case EProjectileState::InitialMovement:
		{
			LineColorSphere = FColor::Green;
			break;
		}
		case EProjectileState::WaitingToTeleportInstigator:
		{
			LineColorSphere = FColor::Blue;
			break;
		}
		default: ;
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), 50.0, 32, LineColorSphere, false);
}
