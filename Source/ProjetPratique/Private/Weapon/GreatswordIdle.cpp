// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GreatswordIdle.h"

#include "Weapon/Blade.h"


AGreatswordIdle::AGreatswordIdle() : Super(EBladeType::EBT_GreatSword)
{
	PrimaryActorTick.bCanEverTick = true;
}

