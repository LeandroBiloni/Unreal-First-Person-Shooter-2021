// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	if (PrefabCanvas) 
	{
		CurrentUI = CreateWidget<UMyUserWidget>(this, PrefabCanvas);

		if (CurrentUI)
		{
			CurrentUI->AddToViewport();
		}
	}
}