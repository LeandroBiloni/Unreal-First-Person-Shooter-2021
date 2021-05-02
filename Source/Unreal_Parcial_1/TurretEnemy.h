// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

# include "Engine.h"
# include "Engine/World.h"
# include "GameFramework/Actor.h"
# include "Bullet.h"
#include "TurretEnemy.generated.h"

UENUM()
enum class EBehavioursEnemy : uint8
{
	BE_Shoot UMETA(DisplayName = "Shoot"),
	BE_LookAt UMETA(DisplayName = "LookAt")
};

UCLASS()
class UNREAL_PARCIAL_1_API ATurretEnemy : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ATurretEnemy();

	AActor* player;

	UPROPERTY(EditAnywhere, Category = Enum)
		EBehavioursEnemy myEnum;

	UPROPERTY(EditAnywhere)
		float rotationSpeed;

	UPROPERTY(EditAnywhere)
		TSubclassOf <class ABullet> bulletPrefab;

	UPROPERTY(EditAnywhere)
		float attackRange;

	UPROPERTY(EditAnywhere)
		float attackSpeed;

	float timer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void LookTarget();
	void Shoot();
};
