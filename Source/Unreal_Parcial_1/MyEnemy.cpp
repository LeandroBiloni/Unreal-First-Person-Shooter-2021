// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere->InitSphereRadius(500.0f);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//Sound
	myAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	DamageOn = false;
	DamageOnCounter = 0;
	MyMesh = FindComponentByClass<USkeletalMeshComponent>();

	CurrentLife = MaxLife;

	//Animation
	if (MyMesh)
	{
		anim = Cast<UEnemyAnimInstance>(MyMesh->GetAnimInstance());
	}

}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (myEnum == EBehavioursEnemy::BE_Dead) return;
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
		if (canAttack)
			Attack();
		break;
	case EBehavioursEnemy::BE_Dead:
		break;
	}

	if (DamageOn)
	{
		DamageOnCounter += DeltaTime;

		if (DamageOnCounter >= DamageMaterialTime)
		{
			DamageOn = false;
			DamageOnCounter = 0;
			CopyMaterial = UMaterialInstanceDynamic::Create(OriginalMaterial, this);
			MyMesh->SetMaterial(MaterialPosToReplace, CopyMaterial);
		}
	}

	myCurrentTime += DeltaTime;

}

void AMyEnemy::TakeDamage(float damage)
{
	CurrentLife -= damage;
	CopyMaterial = UMaterialInstanceDynamic::Create(DamageMaterial, this);
	MyMesh->SetMaterial(MaterialPosToReplace, CopyMaterial);
	DamageOn = true;

	//Sound
	PlaySound(hurtSound);

	//Animation
	if (anim)
	{
		anim->isHit = true;
		if (CurrentLife <= 0)
		{
			anim->isDead = true;
			Sphere->SetSphereRadius(0.0f);
			myEnum = EBehavioursEnemy::BE_Dead;
		}
	}

}

void AMyEnemy::LookTarget()
{
	FVector dir = Player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());

	//Animation
	if (anim)
	{
		anim->isMoving = false;
	}
}

void AMyEnemy::FollowTarget(float deltaTime)
{
	LookTarget();
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);

	FVector dist = Player->GetActorLocation() - GetActorLocation();
	if (dist.Size() > range)
	{
		myEnum = EBehavioursEnemy::BE_LookPlayer;
	}
	//Animation
	if (anim)
		anim->isMoving = true;
}

void AMyEnemy::Avoidance(float deltaTime)
{
	FVector dist = Player->GetActorLocation() - GetActorLocation();
	FVector dir = dist.GetSafeNormal();
	if (ClosestObstacle)
		dir += (GetActorLocation() - ClosestObstacle->GetActorLocation()).GetSafeNormal() * AvoidWeight;

	if (dist.Size() > range)
	{
		myEnum = EBehavioursEnemy::BE_LookPlayer;
	}

	//Animation
	if (anim)
	{
		anim->isMoving = true;
	}

	dir.Z = 0;
	FVector rot = FMath::Lerp(GetActorForwardVector(), dir, SpeedRot * deltaTime);

	SetActorRotation(rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AMyEnemy::Attack()
{
	if (myCurrentTime < attackCooldown)
	{
		myEnum = EBehavioursEnemy::BE_LookPlayer;
		UE_LOG(LogTemp, Warning, TEXT("No Attack"));
		return;
	}

	Player->GetDamage(myDamage);
	myCurrentTime = 0;
	
	//Sound
	PlaySound(attackSound);

	//Animation
	if (anim)
	{
		anim->isAttacking = true;
	}
}

void AMyEnemy::MyBeginOverlap(AActor* overlapActor)
{
	if (overlapActor == this)
		return;
	if (overlapActor == Player)
	{
		FVector distB = overlapActor->GetActorLocation() - GetActorLocation();
		if (distB.Size() <= AttackRange)
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

	}
}

//Sound
void AMyEnemy::PlaySound(USoundWave* sound)
{
	myAudio->Stop();
	myAudio->Sound = sound;
	myAudio->Play();
}