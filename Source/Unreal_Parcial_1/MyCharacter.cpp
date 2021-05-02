// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseX", this, &AMyCharacter::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &AMyCharacter::MouseY);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
}

void AMyCharacter::MoveForward(float f)
{
	//AddMovementInput(GetActorForwardVector(), f);
	if (Controller && f != 0.0)
	{
		FRotator rot = Controller->GetControlRotation();
		FRotator myYaw(0, rot.Yaw, 0);
		FVector dir = FRotationMatrix(myYaw).GetUnitAxis(EAxis::X);
		AddMovementInput(dir, f);
	}
}

void AMyCharacter::MoveRight(float f)
{
	//AddMovementInput(GetActorRightVector(), f);
	if (Controller && f != 0.0)
	{
		FRotator rot = Controller->GetControlRotation();
		FRotator myYaw(0, rot.Yaw, 0);
		FVector dir = FRotationMatrix(myYaw).GetUnitAxis(EAxis::Y);
		AddMovementInput(dir, f);
	}
}

void AMyCharacter::MouseX(float f)
{
	AddControllerYawInput(f);
}

void AMyCharacter::MouseY(float f)
{
	AddControllerPitchInput(f);
}

