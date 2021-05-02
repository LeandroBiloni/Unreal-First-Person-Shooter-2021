// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRaycast.h"

// Sets default values for this component's properties
UGunRaycast::UGunRaycast()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunRaycast::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGunRaycast::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(viewPoint, viewRotation);

	finalPosition = viewPoint + viewRotation.Vector() * distance;

	FHitResult hit;

	FCollisionQueryParams params = FCollisionQueryParams(TEXT(""), false, GetOwner()); 

	GetWorld()->LineTraceSingleByChannel(hit, viewPoint, finalPosition, ECollisionChannel::ECC_PhysicsBody, params);

	if (hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("Golpee algo %s"), *hit.GetActor()->GetName());
	}

	DrawDebugLine(GetWorld(), viewPoint, finalPosition, FColor::Red, false, -1.0f, 0.0f, 10.0f);
}

