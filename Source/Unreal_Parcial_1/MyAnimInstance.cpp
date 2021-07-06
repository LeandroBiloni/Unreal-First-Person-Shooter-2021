// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"


void UMyAnimInstance::SetDirection(FVector2D v)
{
	dir = v;
}

void UMyAnimInstance::EndHit()
{
	isHit = false;
}

void UMyAnimInstance::EndAttack()
{
	isAttacking = false;
}