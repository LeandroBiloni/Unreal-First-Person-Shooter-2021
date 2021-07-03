// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere->InitSphereRadius(250.0f);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	//Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
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
	case EBehavioursEnemy::BE_LookPlayer:
		LookTarget();
		break;
	case EBehavioursEnemy::BE_Follow:
		FollowTarget(DeltaTime);
		break;
	case EBehavioursEnemy::BE_Avoidance:
		Avoidance(DeltaTime);
		break;
	case EBehavioursEnemy::BE_Attack:
		Attack();
		break;
	}
}

void AMyEnemy::TakeDamage(int damage)
{
	CurrentLife -= damage;
}

void AMyEnemy::LookTarget()
{
	FVector dir = Player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());
}

void AMyEnemy::FollowTarget(float deltaTime)
{
	LookTarget();
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AMyEnemy::Avoidance(float deltaTime)
{
	FVector dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	if (ClosestObstacle)
		dir += (GetActorLocation() - ClosestObstacle->GetActorLocation()).GetSafeNormal() * AvoidWeight;

	dir.Z = 0;
	FVector rot = FMath::Lerp(GetActorForwardVector(), dir, SpeedRot * deltaTime);

	SetActorRotation(rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AMyEnemy::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
	if(canAttack)
		Player->GetDamage(myDamage);

	canAttack = false;
}

void AMyEnemy::MyBeginOverlap(AActor* overlapActor)
{
	if (overlapActor == this )
		return;
	if (overlapActor == Player)
	{
		FVector distB = overlapActor->GetActorLocation() - GetActorLocation();
		if (distB.Size() < AttackRange)
		{
			canAttack = true;
			myEnum = EBehavioursEnemy::BE_Attack;
			return;
		}
		myEnum = EBehavioursEnemy::BE_Follow;
	}


	if (ClosestObstacle)
	{
		FVector distA = ClosestObstacle->GetActorLocation() - GetActorLocation();
		FVector distB = overlapActor->GetActorLocation() - GetActorLocation();

		if (distB.Size() < distA.Size())
		{
			ClosestObstacle = overlapActor;
			myEnum = EBehavioursEnemy::BE_Avoidance;

		}
	}
	else
	{
		ClosestObstacle = overlapActor;
		myEnum = EBehavioursEnemy::BE_LookPlayer;
	}
}

