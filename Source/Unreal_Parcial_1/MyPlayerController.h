// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyUserWidget.h"
#include "Engine.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PARCIAL_1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> PrefabCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* CurrentUI;

	virtual void BeginPlay() override;
};
