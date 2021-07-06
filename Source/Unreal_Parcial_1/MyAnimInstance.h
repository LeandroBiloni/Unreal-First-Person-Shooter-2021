// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PARCIAL_1_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		FVector2D dir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isHit;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isDead;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isAttacking;

	UFUNCTION(BlueprintCallable)
		void SetDirection(FVector2D v);

	UFUNCTION(BlueprintCallable)
		void EndHit();
	
	UFUNCTION(BlueprintCallable)
		void EndAttack();
};
