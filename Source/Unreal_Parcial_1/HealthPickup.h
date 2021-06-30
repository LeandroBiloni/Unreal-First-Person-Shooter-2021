// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

UCLASS()
class UNREAL_PARCIAL_1_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int HealAmount;

	UPROPERTY(EditAnywhere)
		float MoveSpeed;

	UPROPERTY(EditAnywhere)
		float RotationSpeed;

	UPROPERTY(EditAnywhere)
	int MaxHeight;

	UPROPERTY(EditAnywhere)
	int MinHeight;

	bool Reverse;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveVertical(float DeltaTime);
	void Rotation(float DeltaTime);

};
