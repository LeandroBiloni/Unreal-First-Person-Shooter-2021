// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Bullet.h"
#include "MySceneManager.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Sound
	MyAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*if (bulletPrefab == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("NO TENGO BALA"));*/
	UE_LOG(LogTemp, Warning, TEXT("BeginPlayWorks"));

	if (bulletPrefab != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletPrefab is Set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BulletPrefab is a NULL POINTER"));
	}

	currentLife = maxLife;
	TakeDamage = false;
	TakeDamageCounter = 0;
	MyMesh = FindComponentByClass<USkeletalMeshComponent>();

	//Sound
	//MyAudio = FindComponentByClass<UAudioComponent>();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
	if (currentTime >= shootTimer)
		canShoot = true;

	if (TakeDamage)
	{
		TakeDamageCounter += DeltaTime;

		if (TakeDamageCounter >= DamageMaterialTime)
		{
			TakeDamage = false;
			TakeDamageCounter = 0;
			CopyMaterial = UMaterialInstanceDynamic::Create(OriginalMaterial, this);
			MyMesh->SetMaterial(MaterialPosToReplace, CopyMaterial);
		}
	}
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
	//Esto
	if (!canShoot) return;
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
	//Esto
	currentTime = 0;
	canShoot = false;

	//Sound
	PlaySound(shootSound);
}


void AMyCharacter::GetDamage(float damage)
{
	currentLife -= damage;
	CopyMaterial = UMaterialInstanceDynamic::Create(DamageMaterial, this);
	MyMesh->SetMaterial(MaterialPosToReplace, CopyMaterial);
	TakeDamage = true;
	//Sound
	PlaySound(hurtSound);
}

void AMyCharacter::AddLife(float value)
{
	currentLife += value;
	if (currentLife > maxLife)
		currentLife = maxLife;
}

void AMyCharacter::PlaySound(USoundWave* sound)
{
	MyAudio->Stop();
	MyAudio->Sound = sound;
	MyAudio->Play();
}
