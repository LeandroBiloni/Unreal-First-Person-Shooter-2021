// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "EnemyDebuff.generated.h"

UENUM()
enum class EEnemyBehaviours : uint8
{
	BE_Follow UMETA(DisplayName = "Follow"),
	BE_LookPlayer UMETA(DisplayName = "Look"),
	BE_Avoidance UMETA(DisplayName = "Avoid"),
	BE_Attack UMETA(DisplayName = "Attack")
};

UCLASS()
class UNREAL_PARCIAL_1_API AEnemyDebuff : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyDebuff();

	USphereComponent* Sphere;
	TArray<AActor*> Overlap;
	AActor* Player;

	UPROPERTY(EditAnywhere, Category = Enum)
		EEnemyBehaviours myEnum;


	UPROPERTY(EditAnywhere)
		float Speed;
	UPROPERTY(EditAnywhere)
		float SpeedRot;
	UPROPERTY(EditAnywhere)
		float AvoidWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* ClosestObstacle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
		int MaxLife;
	UPROPERTY(EditAnywhere)
		int CurrentLife;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void TakeDamage(int damage);
	void LookTarget();
	void FollowTarget(float deltaTime);
	void Avoidance(float deltaTime);
	void Attack();
	UFUNCTION(BlueprintCallable)
		void MyBeginOverlap(AActor* actorOverlap);
};
