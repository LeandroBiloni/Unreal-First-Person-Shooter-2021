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

	//Sound
	myAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

// Called when the game starts or when spawned
void ATurretEnemy::BeginPlay()
{
	Super::BeginPlay();
	player = GetWorld()->GetFirstPlayerController()->GetPawn();
	canShoot = true;
	currentLife = maxLife;

	DamageOn = false;
	DamageOnCounter = 0;
	MyMesh = FindComponentByClass<USkeletalMeshComponent>();

	//Animation
	if (MyMesh)
	{
		anim = Cast<UEnemyAnimInstance>(MyMesh->GetAnimInstance());
	}
}

// Called every frame
void ATurretEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead)
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

	if (canShoot == false) 
	{
		timer = timer + DeltaTime;

		if (timer >= attackSpeed) 
		{
			canShoot = true;
			timer = 0.0f;
		}
	}

	if (InSight(player->GetActorLocation()))
	{
		LookTarget();
		if (canShoot)
		{
				Shoot();
				canShoot = false;
		}
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

void ATurretEnemy::LookTarget()
{
	FVector dir = player->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	SetActorRotation(dir.Rotation());
}

void ATurretEnemy::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("torreta dispara"));
	AActor* actor = this;
	UWorld* world = GetWorld();

	if (world) 
	{
		world->SpawnActor<ABullet>(bulletPrefab, spawn->GetComponentLocation(), cannon->GetComponentRotation());
	}

	//Sound
	PlaySound(attackSound);

	//Animation
	if (anim)
	{
		anim->isAttacking = true;
	}
}

bool ATurretEnemy::InSight(FVector playerPos)
{
	FVector myPos = GetActorLocation();
	float distToPlayer = (myPos - playerPos).Size();

	if (distToPlayer > attackRange) 
	{
		return false;
	}

	FVector dirToPlayer = (myPos - playerPos).GetSafeNormal();
	if (FVector::DotProduct(this->GetActorForwardVector(), dirToPlayer) > attackAngle / 2) 
	{
		return false;
	}

	FHitResult hit;

	FCollisionQueryParams params = FCollisionQueryParams(TEXT(""), false, GetOwner());

	FVector offset = FVector(0.0f, 200.0f, 0.0f);
	GetWorld()->LineTraceSingleByChannel(hit, spawn->GetComponentLocation(), spawn->GetComponentLocation() + spawn->GetForwardVector() * attackRange, ECollisionChannel::ECC_PhysicsBody, params);

	DrawDebugLine(GetWorld(), spawn->GetComponentLocation(), spawn->GetComponentLocation() + spawn->GetForwardVector() * attackRange, FColor::Red, false, -1.0f, 0.0f, 10.0f);
	if (hit.GetActor() != player) 
	{
		return false;
	}
	return true;
}

void ATurretEnemy::TakeDamage(float damage)
{
	currentLife -= damage;
	CopyMaterial = UMaterialInstanceDynamic::Create(DamageMaterial, this);
	MyMesh->SetMaterial(MaterialPosToReplace, CopyMaterial);
	DamageOn = true;

	//Sound
	PlaySound(hurtSound);

	//Animation
	if (anim)
	{
		anim->isHit = true;
		if (currentLife <= 0)
		{
			anim->isDead = true;
			isDead = true;
		}
	}
}

//Sound
void  ATurretEnemy::PlaySound(USoundWave* sound)
{
	myAudio->Stop();
	myAudio->Sound = sound;
	myAudio->Play();
}