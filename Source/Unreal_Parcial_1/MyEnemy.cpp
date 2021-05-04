// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Player) return;
	if (!Sphere) return;

	ClosestObstacle = nullptr;
	Sphere->GetOverlappingActors(Overlap);

	for (&auto item : Overlap)
	{
		MyBeginOverlap(item);
	}

	switch (myEnum)
	{
	case EBehavioursEnemy::BE_LookPlayer:
		LookTarget();
		break;
	case EBehavioursEnemy::BE_Follow:
		FollowTarget(DeltaTime);
		break;
	case EBehavioursEnemy::BE_Avoidance:
		Avoidance(DeltaTime);
		break;
	}
}

void AMyEnemy::LookTarget()
{
	FVector dir = player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());
}

void AMyEnemy::FollowTarget(float deltaTime)
{
	LookTarget();
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * deltaTime);
}

void AMyEnemy::Avoidance(float deltaTime)
{
	FVector dir = (player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	if (closestObstacle)
		dir += (GetActorLocation() - closestObstacle->GetActorLocation()).GetSafeNormal() * avoidWeight;

	dir.Z = 0;
	FVector rot = FMath::Lerp(GetActorForwardVector(), dir, speedRot * deltaTime);

	SetActorRotation(rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * deltaTime);
}

void AMyEnemy::MyBeginOverlap(AActor* overlapActor)
{
	if (overlapActor == this || overlapActor == player)
		return;

	if (closestObstacle)
	{
		FVector distA = closestObstacle->GetActorLocation() - GetActorLocation();
		FVector distB = overlapActor->GetActorLocation() - GetActorLocation();

		if (distB.Size() < distA.Size())
		{
			closestObstacle = overlapActor;
		}
	}
	else
		closestObstacle = overlapActor;
}

void GetDamage(int damage)
{
	CurrentLife -= damage;
}

