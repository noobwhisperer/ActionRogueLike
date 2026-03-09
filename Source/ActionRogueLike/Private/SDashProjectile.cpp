// file: STeleportProjectile.cpp


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"


ASDashProjectile::ASDashProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// TeleportNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("TeleportNiagaraComp");
	// TeleportNiagaraComp->SetupAttachment(SphereComp);
	// TeleportNiagaraComp->Activate(false);
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileState = EProjectileState::InitialMovement;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASDashProjectile::Explode, DetonationTimer);
}


void ASDashProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_StateTimer);

	if (ImpactVfx)
	{
		// Spawn the Niagara effect at the projectile's location and rotation
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());
	}

	if (EffectComp)
	{
		EffectComp->Deactivate();
	}

	ProjectileMoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	ProjectileState = EProjectileState::WaitingToTeleportInstigator;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASDashProjectile::TeleportInstigator, TeleportTimer);


	//Tom Looman stuff :
	//
	// FTimerHandle TimerHandle_DelayedTeleport;
	// GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
	//
	// // Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer)
	// //Super::Explode_Implementation();
}


// void ASDashProjectile::ExplodeProjectile()
// {
// 	// set movement velocity to "zero"
//
// 	MoveComp->Velocity *= 0.0;
// 	//note: seems to not be necessary since the blocking collision stops the projectile anyway
//
// 	// trigger explode/teleport VFX
//
// 	TeleportNiagaraComp->Activate(true);
//
// 	// start teleport timer
//
// 	ProjectileState = EProjectileState::WaitingToTeleportInstigator;
// 	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASDashProjectile::TeleportInstigator, TeleportTimer);
// }

void ASDashProjectile::TeleportInstigator()
{
	AActor* MyInstigator = GetInstigator();
	if (MyInstigator)
	{
		bool bTeleportSuccess = MyInstigator->TeleportTo(GetActorLocation(), MyInstigator->GetActorRotation());
		if (bShowDebugGraphics)
		{
			FColor LineColorSphere = bTeleportSuccess ? FColor::Green : FColor::Blue;
			DrawDebugBox(GetWorld(), GetActorLocation(), FVector(30, 30, 30), LineColorSphere, false, 3.0f);
		}
	}

	//destroy ourselves

	Destroy();
}

void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShowDebugGraphics)
	{
		FColor LineColorSphere = FColor::White;
		switch (ProjectileState)
		{
			case EProjectileState::Unknown:
			{
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
}
