// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

# include "Engine.h"
# include "Engine/World.h"
# include "GameFramework/Actor.h"
# include "EnemyAnimInstance.h"
# include "Bullet.h"
#include "TurretEnemy.generated.h"



UCLASS()
class UNREAL_PARCIAL_1_API ATurretEnemy : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ATurretEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* turretBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* cannon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* spawn;

	AActor* player;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* CannonComp;

	UPROPERTY(EditAnywhere)
		float maxLife = 10.0f;

	UPROPERTY(EditAnywhere)
		float currentLife;

	UPROPERTY(EditAnywhere)
		float rotationSpeed;

	UPROPERTY(EditAnywhere)
		TSubclassOf <class ABullet> bulletPrefab;

	UPROPERTY(EditAnywhere)
		float attackRange;

	UPROPERTY(EditAnywhere)
		float attackAngle;

	UPROPERTY(EditAnywhere)
		float attackSpeed;

	float timer;

	bool canShoot;

	bool isDead;

	//Material
	UPROPERTY(EditAnywhere)
		int MaterialPosToReplace;

	USkeletalMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* OriginalMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* DamageMaterial;

	UMaterialInstanceDynamic* CopyMaterial;

	bool DamageOn;

	float DamageOnCounter;

	UPROPERTY(EditAnywhere)
		float DamageMaterialTime;

	//Sound
	UPROPERTY(EditAnywhere)
		USoundWave* attackSound;
	UPROPERTY(EditAnywhere)
		USoundWave* hurtSound;
	UPROPERTY(EditAnywhere)
		USoundWave* dieSound;
	UAudioComponent* myAudio;

	//Animation
	UEnemyAnimInstance* anim;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void LookTarget();
	void Shoot();
	bool InSight(FVector playerPos);

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float damage);

	//Sound
	void PlaySound(USoundWave* sound);
};
