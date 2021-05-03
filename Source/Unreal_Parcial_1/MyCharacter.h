// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class UNREAL_PARCIAL_1_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	int maxLife;
	int currentLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABullet> bulletPrefab;

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Level name")
	FName LevelA;
	UPROPERTY(EditDefaultsOnly, Category = "Level name")
	FName LevelB;
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float f);
	void MoveRight(float f);
	void MouseX(float f);
	void MouseY(float f);
	void Shoot();

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeLevel(const FName& LvlToChange);

	void RestartThisLevel();

	void TakeDamage(int damage);
};
