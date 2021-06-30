// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDebuff.h"

// Sets default values
AEnemyDebuff::AEnemyDebuff()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere->InitSphereRadius(250.0f);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AEnemyDebuff::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void AEnemyDebuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Player) return;
	if (!Sphere) return;

	ClosestObstacle = nullptr;
	Sphere->GetOverlappingActors(Overlap);

	for (auto& item : Overlap)
	{
		MyBeginOverlap(item);
	}

	switch (myEnum)
	{
	case EEnemyBehaviours::BE_LookPlayer:
		LookTarget();
		break;
	case EEnemyBehaviours::BE_Follow:
		FollowTarget(DeltaTime);
		break;
	case EEnemyBehaviours::BE_Avoidance:
		Avoidance(DeltaTime);
		break;
	}
}

void AEnemyDebuff::TakeDamage(int damage)
{
	CurrentLife -= damage;
}

void AEnemyDebuff::LookTarget()
{
	FVector dir = Player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());
}

void AEnemyDebuff::FollowTarget(float deltaTime)
{
	LookTarget();
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AEnemyDebuff::Avoidance(float deltaTime)
{
	FVector dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	if (ClosestObstacle)
		dir += (GetActorLocation() - ClosestObstacle->GetActorLocation()).GetSafeNormal() * AvoidWeight;

	dir.Z = 0;
	FVector rot = FMath::Lerp(GetActorForwardVector(), dir, SpeedRot * deltaTime);

	SetActorRotation(rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AEnemyDebuff::MyBeginOverlap(AActor* overlapActor)
{
	if (overlapActor == this)
		return;

	if (overlapActor == Player)
	{
		myEnum = EEnemyBehaviours::BE_Follow;
	}

	if (ClosestObstacle)
	{
		FVector distA = ClosestObstacle->GetActorLocation() - GetActorLocation();
		FVector distB = overlapActor->GetActorLocation() - GetActorLocation();

		if (distB.Size() < distA.Size())
		{
			ClosestObstacle = overlapActor;
			myEnum = EEnemyBehaviours::BE_Avoidance;
		}
	}
	else
	{
		ClosestObstacle = overlapActor;
		myEnum = EEnemyBehaviours::BE_LookPlayer;
	}
}
