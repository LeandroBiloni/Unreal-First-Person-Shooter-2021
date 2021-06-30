// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	Reverse = true;
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveVertical(DeltaTime);
	Rotation(DeltaTime);

}

void AHealthPickup::MoveVertical(float DeltaTime)
{
	float pos = GetActorLocation().Z;
		
	if (Reverse)
		SetActorLocation(GetActorLocation() + GetActorUpVector() * -1 * MoveSpeed * DeltaTime);
	else SetActorLocation(GetActorLocation() + GetActorUpVector() * MoveSpeed * DeltaTime);
	if (pos > MaxHeight) Reverse = true;

	if (pos < MinHeight) Reverse = false;
}

void AHealthPickup::Rotation(float DeltaTime)
{
	this->AddActorLocalRotation(FRotator(0.0f, 180.0f, 0.0f) * RotationSpeed * DeltaTime);
}

