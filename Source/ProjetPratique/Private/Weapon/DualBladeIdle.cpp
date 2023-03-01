// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/DualBladeIdle.h"

#include "Weapon/Blade.h"

ADualBladeIdle::ADualBladeIdle() : ABladeIdle(EBladeType::EBT_Dual)
{
	PrimaryActorTick.bCanEverTick = true;
	OtherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Other Mesh"));
	OtherMesh->SetupAttachment(GetRootComponent());
}
