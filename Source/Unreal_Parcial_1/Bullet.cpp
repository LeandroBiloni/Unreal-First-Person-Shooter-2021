// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
//#include "MyCharacter.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	MyPlayer = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	Owner = GetOwner();

	if (Player == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("player null"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("player no es null"));
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
}

void ABullet::ChangeToFire()
{
	UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
	if (Mesh) 
	{
		CopyMaterial = UMaterialInstanceDynamic::Create(FireMaterial, this);
		Mesh->SetMaterial(0, CopyMaterial);
	}
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//if (OtherActor != this && OtherActor == Player)
	//{
	//	
	//}
	//UE_LOG(LogTemp, Warning, TEXT("choque"));
	//Destroy();
	
}

void ABullet::DamageCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("daño al player"));
	MyPlayer->GetDamage(Damage);
}
