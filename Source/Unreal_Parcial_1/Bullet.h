// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MyCharacter.h"
#include "Bullet.generated.h"

UCLASS()
class UNREAL_PARCIAL_1_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	UPROPERTY(EditAnywhere)
		float Speed;
	UPROPERTY(EditAnywhere)
		float Damage;

	TSubclassOf <class AMyCharacter> MyPlayer;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* MyTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
