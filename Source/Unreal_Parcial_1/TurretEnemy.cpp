// Fill out your copyright notice in the Description page of Project Settings.


# include "TurretEnemy.h"

// Sets default values
ATurretEnemy::ATurretEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurretEnemy::BeginPlay()
{
	Super::BeginPlay();
	player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void ATurretEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (myEnum)
	{
	case EBehavioursEnemy::BE_LookAt:
		LookTarget();
		break;

	case EBehavioursEnemy::BE_Shoot:
		Shoot();
		break;
	}
}

void ATurretEnemy::LookTarget()
{
	FVector dir = player->GetActorLocation() - GetActorLocation();
	SetActorRotation(dir.Rotation());
}

void ATurretEnemy::Shoot()
{
	if (bulletPrefab)
	{
		GetWorld()->SpawnActor<ABullet>(bulletPrefab, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
}

