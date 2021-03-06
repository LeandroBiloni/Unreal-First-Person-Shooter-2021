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

	//Sound
	myAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

// Called when the game starts or when spawned
void AEnemyDebuff::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	CurrentLife = MaxLife;

	DamageOn = false;
	DamageOnCounter = 0;
	MyMesh = FindComponentByClass<USkeletalMeshComponent>();

	//Animation
	if (MyMesh)
		anim = Cast<UEnemyAnimInstance>(MyMesh->GetAnimInstance());

}

// Called every frame
void AEnemyDebuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (myEnum == EEnemyBehaviours::BE_Dead) 
	{
		UE_LOG(LogTemp, Warning, TEXT("EMPIEZO TIMER %f"), DeadTime);
		DeadTime += DeltaTime;
		if (DeadTime >= 5)
		{
			UE_LOG(LogTemp, Warning, TEXT("DESTROY"));
			Destroy();
		}
		return;
	}
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
	case EEnemyBehaviours::BE_Attack:
		Attack();
		break;
	case EEnemyBehaviours::BE_Dead:
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
}

void AEnemyDebuff::TakeDamage(float damage)
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
			Player->Debuff(false);
			Sphere->SetSphereRadius(0.0f);
			myEnum = EEnemyBehaviours::BE_Dead;
		}
	}
	

}

void AEnemyDebuff::LookTarget()
{
	FVector dir = Player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());

	//Animation
	if (anim)
		anim->isMoving = false;

	if (dir.Size() <= range)
	{
		myEnum = EEnemyBehaviours::BE_Follow;
	}
}

void AEnemyDebuff::FollowTarget(float deltaTime)
{
	LookTarget();
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);

	//Animation
	if (anim)
	{
		anim->isMoving = true;
	}

	FVector distToPlayer = Player->GetActorLocation() - GetActorLocation();
	if (distToPlayer.Size() > range)
	{
		myEnum = EEnemyBehaviours::BE_LookPlayer;
	}
	
	if (distToPlayer.Size() <= AttackRange)
	{
		myEnum = EEnemyBehaviours::BE_Attack;
	}
}

void AEnemyDebuff::Avoidance(float deltaTime)
{
	FVector distToPlayer = Player->GetActorLocation() - GetActorLocation();
	FVector dir = distToPlayer.GetSafeNormal();
	if (ClosestObstacle)
		dir += (GetActorLocation() - ClosestObstacle->GetActorLocation()).GetSafeNormal() * AvoidWeight;

	//Animation
	if (anim)
	{
		anim->isMoving = true;
	}

	dir.Z = 0;
	FVector rot = FMath::Lerp(GetActorForwardVector(), dir, SpeedRot * deltaTime);

	if (distToPlayer.Size() > range)
	{
		myEnum = EEnemyBehaviours::BE_LookPlayer;
		return;
	}

	SetActorRotation(rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AEnemyDebuff::Attack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack"));

	//Logica de debuff
	Player->Debuff(true);
	//Sound
	PlaySound(attackSound);

	//Animation
	if (anim)
	{
		anim->isAttacking = true;
	}

}

void AEnemyDebuff::MyBeginOverlap(AActor* overlapActor)
{
	if (overlapActor == this)
		return;

	if (overlapActor == Player)
	{
		return;
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

//Sound
void AEnemyDebuff::PlaySound(USoundWave* sound)
{
	myAudio->Stop();
	myAudio->Sound = sound;
	myAudio->Play();
}