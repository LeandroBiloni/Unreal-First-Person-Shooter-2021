// Fill out your copyright notice in the Description page of Project Settings.


#include "MySceneManager.h"

// Sets default values
AMySceneManager::AMySceneManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySceneManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMySceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMySceneManager::LoadFirstLevel() 
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lvl1");
}

void AMySceneManager::LoadSecondLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lvl2");
}

void AMySceneManager::LoadThirdLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lvl3");
}

void AMySceneManager::LoadFourthLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lvl4");
}

void AMySceneManager::Win()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Win");
}

void AMySceneManager::Lose()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Lose");
}
