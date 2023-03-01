// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/DualBlade.h"
#include "Components/StaticMeshComponent.h"


ADualBlade::ADualBlade()
{
	BladeType = EBladeType::EBT_Dual;

	OtherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Mesh"));
	OtherMesh->SetupAttachment(GetRootComponent());
}



