// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStunner.h"

// Sets default values
AEnemyStunner::AEnemyStunner()
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
void AEnemyStunner::BeginPlay()
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
void AEnemyStunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (myEnum == EEnemyStunnerBehaviours::BE_Dead || anim->isDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("EMPIEZO TIMER %f"), DeadTime);
		Sphere->SetSphereRadius(0.0f);
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
	case EEnemyStunnerBehaviours::BE_LookPlayer:
		LookTarget();
		break;
	case EEnemyStunnerBehaviours::BE_Follow:
		FollowTarget(DeltaTime);
		break;
	case EEnemyStunnerBehaviours::BE_Avoidance:
		Avoidance(DeltaTime);
		break;
	case EEnemyStunnerBehaviours::BE_Attack:
		if(!attacked)
			Attack();
		break;
	case EEnemyStunnerBehaviours::BE_Dead:
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

void AEnemyStunner::TakeDamage(float damage)
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
			myEnum = EEnemyStunnerBehaviours::BE_Dead;
		}
	}
}

void AEnemyStunner::LookTarget()
{
	FVector dir = Player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());

	//Animation
	if (anim)
		anim->isMoving = false;

	if (dir.Size() <= range)
	{
		myEnum = EEnemyStunnerBehaviours::BE_Follow;
	}
}

void AEnemyStunner::FollowTarget(float deltaTime)
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
		myEnum = EEnemyStunnerBehaviours::BE_LookPlayer;
	}

	if (distToPlayer.Size() <= AttackRange)
	{
		myEnum = EEnemyStunnerBehaviours::BE_Attack;
	}
}

void AEnemyStunner::Avoidance(float deltaTime)
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
		myEnum = EEnemyStunnerBehaviours::BE_LookPlayer;
		return;
	}

	SetActorRotation(rot.Rotation());
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * deltaTime);
}

void AEnemyStunner::Attack()
{
	//logica de ataque que estunee al player.
	Player->GetDamage(myDamage);
	Player->StunThisUnit();

	if (anim)
	{
		anim->isAttacking = true;
	}
	attacked = true;
}

void AEnemyStunner::MyBeginOverlap(AActor * actorOverlap)
{
	if (actorOverlap == this)
		return;

	if (actorOverlap == Player)
	{
		return;
	}

	if (ClosestObstacle)
	{
		FVector distA = ClosestObstacle->GetActorLocation() - GetActorLocation();
		FVector distB = actorOverlap->GetActorLocation() - GetActorLocation();

		if (distB.Size() < distA.Size())
		{
			ClosestObstacle = actorOverlap;
			myEnum = EEnemyStunnerBehaviours::BE_Avoidance;
		}
	}
	else
	{
		ClosestObstacle = actorOverlap;
		myEnum = EEnemyStunnerBehaviours::BE_LookPlayer;
	}
}

void AEnemyStunner::PlaySound(USoundWave * sound)
{
	myAudio->Stop();
	myAudio->Sound = sound;
	myAudio->Play();
}
