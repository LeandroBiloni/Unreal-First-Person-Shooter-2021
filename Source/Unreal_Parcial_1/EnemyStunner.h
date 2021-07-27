// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "EnemyAnimInstance.h"
#include "MyCharacter.h"
#include "EnemyStunner.generated.h"

UENUM()
enum class EEnemyStunnerBehaviours : uint8
{
	BE_Follow UMETA(DisplayName = "Follow"),
	BE_LookPlayer UMETA(DisplayName = "Look"),
	BE_Avoidance UMETA(DisplayName = "Avoid"),
	BE_Attack UMETA(DisplayName = "Attack"),
	BE_Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class UNREAL_PARCIAL_1_API AEnemyStunner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyStunner();

	USphereComponent* Sphere;
	TArray<AActor*> Overlap;
	AMyCharacter* Player;

	UPROPERTY(EditAnywhere, Category = Enum)
		EEnemyStunnerBehaviours myEnum;

	UPROPERTY(EditAnywhere)
		float Speed;
	UPROPERTY(EditAnywhere)
		float SpeedRot;
	UPROPERTY(EditAnywhere)
		float AvoidWeight;
	UPROPERTY(EditAnywhere)
		float range = 400.0f;
	UPROPERTY(EditAnywhere)
		float AttackRange = 30.0f;
	UPROPERTY(EditAnywhere)
		float myDamage = 3.0f;
	UPROPERTY(BlueprintReadWrite)
		AActor* ClosestObstacle;
	
	bool attacked;

	//TakeDamage
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
		float MaxLife;
	UPROPERTY(EditAnywhere)
		float CurrentLife;
	float DeadTime;
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
