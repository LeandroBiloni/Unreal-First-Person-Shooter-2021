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
	originalShootTimer = shootTimer;
	currentTime = shootTimer;
	currentLife = maxLife;
	TakeDamage = false;
	TakeDamageCounter = 0;
	MyMesh = FindComponentByClass<USkeletalMeshComponent>();

	//Animation
	USkeletalMeshComponent* sk = FindComponentByClass< USkeletalMeshComponent>();
	if (sk)
		anim = Cast<UMyAnimInstance>(sk->GetAnimInstance());
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!canShoot)
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

	//Animacion
	anim->SetDirection(myDir);
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
		//Animacion
		myDir.Y = f;
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
		//Animacion
		myDir.X = f;
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
	UE_LOG(LogTemp, Warning, TEXT("Shoot check"));

	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector MuzzleLoaction = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	FRotator MuzzleRotation = CameraRotation;

	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ABullet* bullet = World->SpawnActor<ABullet>(bulletPrefab, MuzzleLoaction, MuzzleRotation, SpawnParams);

		if (FireActive)
		{
			bullet->Damage = FireShotsDamage;
			bullet->ChangeToFire();
			FireShotsAmount--;

			if (FireShotsAmount == 0)
			{
				FireActive = false;
			}
		}
	}
	//Esto
	UE_LOG(LogTemp, Warning, TEXT("current time a 0"));
	currentTime = 0;
	canShoot = false;

	//Sound
	PlaySound(shootSound);

	//Animation
	anim->isAttacking = true;
}

void AMyCharacter::Debuff(bool isDebuff)
{
	if (isDebuff)
		shootTimer = debuffShootTimer;
	else
		shootTimer = originalShootTimer;

	//UE_LOG(LogTemp, Warning, TEXT("ShootTimer = %s"), (isDebuff ? TEXT("Debuff True") : TEXT("Debuff False")));
	//UE_LOG(LogTemp, Warning, TEXT("ShootTimer = %f"), shootTimer);

}


void AMyCharacter::GetDamage(float damage)
{
	currentLife -= damage;
	CopyMaterial = UMaterialInstanceDynamic::Create(DamageMaterial, this);
	MyMesh->SetMaterial(MaterialPosToReplace, CopyMaterial);
	TakeDamage = true;
	//Sound
	PlaySound(hurtSound);
	//Animacion
	anim->isHit = true;
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