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
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMyCharacter::Shoot);
	//PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AMyCharacter::Restart);
}

void AMyCharacter::MoveForward(float f)
{
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

void AMyCharacter::Shoot()
{
	UE_LOG(LogTemp, Warning,TEXT("Shoot check"));

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	FVector MuzzleLoaction = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	FRotator MuzzleRotation = CameraRotation;

	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ABullet* bullet = World->SpawnActor<ABullet>(bulletPrefab, MuzzleLoaction, MuzzleRotation, SpawnParams);
	}
}

/*void AMyCharacter::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lvl1");
}*/

void AMyCharacter::TakeDamage(int damage)
{
	currentLife -= damage;
}

