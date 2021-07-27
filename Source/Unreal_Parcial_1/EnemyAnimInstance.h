// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PARCIAL_1_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isHit;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isDead;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isAttacking;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool isMoving;

	UFUNCTION(BlueprintCallable)
		void EndMove();

	UFUNCTION(BlueprintCallable)
		void EndHit();

	UFUNCTION(BlueprintCallable)
		void EndAttack();

	UFUNCTION(BlueprintCallable)
		void Die();
};
