// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class UNREAL_PARCIAL_1_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float maxLife;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float currentLife;

	//Este cambio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float shootTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool FireActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int FireShotsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int FireShotsDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float debuffShootTimer = 3.0f;
	float originalShootTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		//Este cambio
		float currentTime;

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

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
		bool TakeDamage;

	float TakeDamageCounter;

	UPROPERTY(EditAnywhere)
		float DamageMaterialTime;

	//Sounds
	UPROPERTY(EditAnywhere)
		USoundWave* shootSound;
	UPROPERTY(EditAnywhere)
		USoundWave* hurtSound;
	UPROPERTY(EditAnywhere)
		USoundWave* dieSound;

	UAudioComponent* MyAudio;

	//Animation
	UMyAnimInstance* anim;
	FVector2D myDir;

	//Para el UI
	class AMyPlayerController* MyPlayerControllerReference;

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

	void GetDamage(float damage);
	void Debuff(bool isDebuff);

	UFUNCTION(BlueprintCallable)
		void AddLife(float value);

	//Sound
	void PlaySound(USoundWave* sound);

	void UpdateLifeBarUI();
	void UpdateCooldownBarUI();

	UFUNCTION(BlueprintCallable)
		void ActivateFirePower(int ShotsAmount, int ShotsDamage);
};