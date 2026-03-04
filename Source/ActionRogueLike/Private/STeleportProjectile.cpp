// file: STeleportProjectile.cpp


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"


ASTeleportProjectile::ASTeleportProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	TeleportNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("TeleportNiagaraComp");
	TeleportNiagaraComp->SetupAttachment(SphereComp);
	TeleportNiagaraComp->Activate(false);

	MotionTimer = 0.2f;
	TeleportTimer = 0.2f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileState = EProjectileState::InitialMovement;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASTeleportProjectile::ExplodeProjectile, MotionTimer);
}

void ASTeleportProjectile::ExplodeProjectile()
{
	// set movement velocity to "zero"

	MovementComp->Velocity *= 0.0;
	//note: seems to not be necessary since the blocking collision stops the projectile anyway

	// trigger explode/teleport VFX

	TeleportNiagaraComp->Activate(true);

	// start teleport timer

	ProjectileState = EProjectileState::WaitingToTeleportInstigator;
	GetWorldTimerManager().SetTimer(TimerHandle_StateTimer, this, &ASTeleportProjectile::TeleportInstigator, TeleportTimer);
}

void ASTeleportProjectile::TeleportInstigator()
{
	AActor* MyInstigator = GetInstigator();
	if (MyInstigator)
	{
		bool bTeleportSuccess = MyInstigator->TeleportTo(GetActorLocation(), MyInstigator->GetActorRotation());
		if(bShowDebugGraphics)
		{
			FColor LineColorSphere = bTeleportSuccess ? FColor::Green : FColor::Blue;
			DrawDebugBox(GetWorld(),GetActorLocation(), FVector(30,30,30), LineColorSphere, false, 3.0f );
		}
	}

	//destroy ourselves

	Destroy();
}

void ASTeleportProjectile::Tick(float DeltaTime)
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
