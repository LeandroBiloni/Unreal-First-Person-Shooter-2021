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
	UPROPERTY(EditAnywhere)
	int maxLife;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int currentLife;
	
	//Este cambio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shootTimer;

	bool canShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABullet> bulletPrefab;

	UPROPERTY(EditAnywhere)
		int MaterialPosToReplace;
	
	USkeletalMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* OriginalMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* DamageMaterial;

	UMaterialInstanceDynamic* CopyMaterial;

	bool TakeDamage;

	float TakeDamageCounter;

	UPROPERTY(EditAnywhere)
		float DamageMaterialTime;

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditDefaultsOnly, Category = "Level name")
	FName LevelA;
	UPROPERTY(EditDefaultsOnly, Category = "Level name")
	FName LevelB;
	
	//Este cambio
	float currentTime;

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

	void GetDamage(int damage);

	UFUNCTION(BlueprintCallable)
		void AddLife(int value);
};
