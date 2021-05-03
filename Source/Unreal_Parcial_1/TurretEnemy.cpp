// Fill out your copyright notice in the Description page of Project Settings.


# include "TurretEnemy.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATurretEnemy::ATurretEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	turretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("turretBody"));
	SetRootComponent(turretBody);

	cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cannon"));
	cannon->SetupAttachment(turretBody);
}

// Called when the game starts or when spawned
void ATurretEnemy::BeginPlay()
{
	Super::BeginPlay();
	player = GetWorld()->GetFirstPlayerController()->GetPawn();
	canShoot = true;
}

// Called every frame
void ATurretEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookTarget();

	FVector playerPos = player->GetActorLocation();
	FVector myPos = GetActorLocation();

	if (canShoot == false) 
	{
		timer = timer + DeltaTime;

		if (timer >= attackSpeed) 
		{
			canShoot = true;
			timer = 0.0f;
		}
	}

	if ((myPos - playerPos).Size() <= attackRange) 
	{
		if (canShoot) 
		{
			UE_LOG(LogTemp, Warning, TEXT("CAMBIO A SHOOT"));
			Shoot();
			canShoot = false;
		}
		
	}
	else UE_LOG(LogTemp, Warning, TEXT("FUERA DE RANGO"));
}

void ATurretEnemy::LookTarget()
{
	FVector dir = player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());
}

void ATurretEnemy::Shoot()
{
	AActor* actor = this;
	//FVector forward = GetActorForwardVector();
	//FVector bulletSpawn = forward + spawnOffset;
	UWorld* world = GetWorld();

	if (world) 
	{
		if (bulletPrefab)
			UE_LOG(LogTemp, Warning, TEXT("TENGO UNA BALA"));

		if (actor)
			UE_LOG(LogTemp, Warning, TEXT("TENGO UN ACTOR"));



		UE_LOG(LogTemp, Warning, TEXT("INSTANCIO BALA"));
		world->SpawnActor<ABullet>(bulletPrefab, cannon->GetComponentLocation(), cannon->GetComponentRotation());
	}
}

