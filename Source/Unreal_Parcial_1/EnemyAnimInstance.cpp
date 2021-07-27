// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::EndMove()
{
	isMoving = false;
}

void UEnemyAnimInstance::EndHit()
{
	isHit = false;
}

void UEnemyAnimInstance::EndAttack()
{
	isAttacking = false;
}

void UEnemyAnimInstance::Die()
{
	isDead = true;
}