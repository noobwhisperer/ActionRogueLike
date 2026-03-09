// file: STeleportProjectile.cpp


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"


ASDashProjectile::ASDashProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileState = EProjectileState::InitialMovement;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASDashProjectile::Explode, DetonationTimer);
}

void ASDashProjectile::Explode_Implementation()
{
	// clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_StateTimer);

	if (ImpactVfx)
	{
		// spawn the Niagara ImpactVfx effect at the projectile's location and rotation
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());
	}

	// deactivate the projectile's movement and collision, but keep the projectile alive until we teleport the instigator
	if (EffectComp)
	{
		EffectComp->Deactivate();
	}

	ProjectileMoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	ProjectileState = EProjectileState::WaitingToTeleportInstigator;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASDashProjectile::TeleportInstigator, TeleportTimer);
}

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

	//destroy ourselves (note: he left this out, but I'm sure we need this)
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
