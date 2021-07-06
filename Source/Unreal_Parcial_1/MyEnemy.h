// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MyCharacter.h"
#include "EnemyAnimInstance.h"
#include "MyEnemy.generated.h"

UENUM()
enum class EBehavioursEnemy : uint8
{
	BE_Follow UMETA(DisplayName = "Follow"),
	BE_LookPlayer UMETA(DisplayName = "Look"),
	BE_Avoidance UMETA(DisplayName = "Avoid"),
	BE_Attack UMETA(DisplayName = "Attack"),
	BE_Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class UNREAL_PARCIAL_1_API AMyEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyEnemy();

	USphereComponent* Sphere;
	TArray<AActor*> Overlap;
	AMyCharacter* Player;

	UPROPERTY(EditAnywhere, Category = Enum)
		EBehavioursEnemy myEnum;


	UPROPERTY(EditAnywhere)
		float Speed;
	UPROPERTY(EditAnywhere)
		float SpeedRot;
	UPROPERTY(EditAnywhere)
		float AvoidWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* ClosestObstacle;
	UPROPERTY(EditAnywhere)
		float range = 250.0f;
	UPROPERTY(EditAnywhere)
		float AttackRange = 50.0f;
	UPROPERTY(EditAnywhere)
		float myDamage;
	UPROPERTY(EditAnywhere)
		float attackCooldown;

	float myCurrentTime;

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
	UPROPERTY(EditAnywhere)
		int MaxLife;
	UPROPERTY(EditAnywhere)
		int CurrentLife;
	float DeadTime;

	bool canAttack;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float damage);
	void LookTarget();
	void FollowTarget(float deltaTime);
	void Avoidance(float deltaTime);
	void Attack();
	UFUNCTION(BlueprintCallable)
		void MyBeginOverlap(AActor* actorOverlap);

	//Sound
	void PlaySound(USoundWave* sound);

};