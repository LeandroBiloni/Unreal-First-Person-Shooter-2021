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

	spawn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("spawn"));
	spawn->SetupAttachment(cannon);
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

	

	if (canShoot == false) 
	{
		timer = timer + DeltaTime;

		if (timer >= attackSpeed) 
		{
			canShoot = true;
			timer = 0.0f;
		}
	}

	if (InSight(player->GetActorLocation()) &&  canShoot) 
	{
		UE_LOG(LogTemp, Warning, TEXT("CAMBIO A SHOOT"));
		Shoot();
		canShoot = false;
	}
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
	UWorld* world = GetWorld();

	if (world) 
	{
		UE_LOG(LogTemp, Warning, TEXT("INSTANCIO BALA"));

		world->SpawnActor<ABullet>(bulletPrefab, cannon->GetComponentLocation(), cannon->GetComponentRotation());
	}
}

bool ATurretEnemy::InSight(FVector playerPos)
{
	FVector myPos = GetActorLocation();
	float distToPlayer = (myPos - playerPos).Size();

	if (distToPlayer > attackRange) 
	{
		UE_LOG(LogTemp, Warning, TEXT("FUERA DE RANGO"));
		return false;
	}

	FVector dirToPlayer = (myPos - playerPos).GetSafeNormal();
	if (FVector::DotProduct(this->GetActorForwardVector(), dirToPlayer) > attackAngle / 2) 
	{
		UE_LOG(LogTemp, Warning, TEXT("FUERA DE ANGULO"));
		return false;
	}

	FHitResult hit;

	FCollisionQueryParams params = FCollisionQueryParams(TEXT(""), false, GetOwner());

	FVector offset = FVector(0.0f, 200.0f, 0.0f);
	GetWorld()->LineTraceSingleByChannel(hit, spawn->GetComponentLocation(), spawn->GetComponentLocation() + (dirToPlayer * attackRange), ECollisionChannel::ECC_PhysicsBody, params);

	DrawDebugLine(GetWorld(), spawn->GetComponentLocation(), spawn->GetComponentLocation() + (dirToPlayer * attackRange), FColor::Red, false, -1.0f, 0.0f, 10.0f);
	if (hit.GetActor() != player) 
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ES PLAYER"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("PLAYER INSIGHT"));
	return true;
}

