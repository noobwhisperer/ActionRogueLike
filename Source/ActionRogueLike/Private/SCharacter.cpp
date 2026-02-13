// file: SCharacter.cpp

#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");

	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{

	AddMovementInput(GetActorForwardVector(), Value);

}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//TSH: Note - this is the old (deprecated) input approach. Implement as shown in Looman, but
	//	replace with Enhanced Input approach once this is working.

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

}

