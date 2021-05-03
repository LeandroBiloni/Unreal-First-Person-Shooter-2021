// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

# include "Engine.h"
# include "Engine/World.h"
# include "GameFramework/Actor.h"
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

	AActor* player;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* CannonComp;

	UPROPERTY(EditAnywhere)
		float rotationSpeed;

	UPROPERTY(EditAnywhere)
		TSubclassOf <class ABullet> bulletPrefab;

	UPROPERTY(EditAnywhere)
		float attackRange;

	UPROPERTY(EditAnywhere)
		float attackSpeed;

	float timer;

	bool canShoot;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void LookTarget();
	void Shoot();
};
