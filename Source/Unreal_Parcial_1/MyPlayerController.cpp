// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FString Name = World->GetName();
	if (Name != "Menu" && Name != "Win" && Name != "Lose")
	{
		SetHUDOnScreen();
		SetActiveFirePowerUpHud(ESlateVisibility::Hidden);
		SetStunHud(ESlateVisibility::Hidden);
	}
	
	/*UWorld* World = GetWorld();
	FString Name = World->GetName();
	if (PrefabCanvas && Name != "Menu" && Name != "Win" && Name != "Lose")
	{
		
		CurrentUI = CreateWidget<UMyUserWidget>(this, PrefabCanvas);

		if (CurrentUI)
		{
			CurrentUI->AddToViewport();
		}
	}*/
}