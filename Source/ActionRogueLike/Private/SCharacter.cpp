// file: SCharacter.cpp

#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// NOTE: UE COORDINATE SYSTEM (LEFT HANDED)
//	X - Forward (red)
//	Y - Right (green)
//	Z - Up (blue)

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");


	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;


	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimMontage);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if(!ensure(ProjectileClass)) return;

	FTransform SpawnTM = ComputeProjectileLaunchTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnimMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	if(!ensure(BlackHoleProjectialClass)) return;

	FTransform SpawnTM = ComputeProjectileLaunchTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(BlackHoleProjectialClass, SpawnTM, SpawnParams);
}

void ASCharacter::TeleportAttack()
{
	PlayAnimMontage(AttackAnimMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::TeleportAttack_TimeElapsed, 0.2f);
}

void ASCharacter::TeleportAttack_TimeElapsed()
{
	if(!ensure(TeleportProjectialClass)) return;

	FTransform SpawnTM = ComputeProjectileLaunchTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(TeleportProjectialClass, SpawnTM, SpawnParams);
}


void ASCharacter::PrimaryInteract()
{
	check(InteractionComp); //note: I cho

	InteractionComp->PrimaryInteract();
}

FTransform ASCharacter::ComputeProjectileLaunchTransform() const
{
	// updated code for lecture 6.5 , adjusting for proper aim with camera "target"

	FHitResult OutHitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	//note - need to ignore "self" probably

	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector CameraDirection = CameraComp->GetForwardVector();
	float TargetDistance = 3000.0f;

	FVector TargetLocation = CameraLocation + TargetDistance * CameraDirection;

	GetWorld()->LineTraceSingleByObjectType(OutHitResult, CameraLocation, TargetLocation, ObjectQueryParams);

	if (OutHitResult.bBlockingHit)
	{
		TargetLocation = OutHitResult.ImpactPoint;
	}

	FVector RightHandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector ProjectileForward = TargetLocation - RightHandLocation;
	ProjectileForward.Normalize();

	FRotator ProjectileRotation = FRotationMatrix::MakeFromXZ(ProjectileForward, FVector(0.0f,0.0f,1.0f)).Rotator();



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

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//TSH: Note - this is the old (deprecated) input approach. Implement as shown in Looman, but
	//	replace with Enhanced Input approach once this is working.

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("TeleportAttack", IE_Pressed, this, &ASCharacter::TeleportAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}
