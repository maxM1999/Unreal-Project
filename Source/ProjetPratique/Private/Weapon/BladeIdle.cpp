// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BladeIdle.h"

#include "Weapon/Blade.h"
#include "Components/CapsuleComponent.h"
#include "Character/SoulsCharacter.h"
#include "TimerManager.h"

ABladeIdle::ABladeIdle() : ABladeIdle(EBladeType::EBT_Dual)
{
}

ABladeIdle::ABladeIdle(EBladeType Type)
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MainMesh->SetupAttachment(GetRootComponent());

	BladeType = Type;
}

void ABladeIdle::BeginPlay()
{
	Super::BeginPlay();

	if(Capsule)
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ABladeIdle::OnCapsuleBeginOverlap);
	}
}

void ABladeIdle::SetActive(bool State)
{
	if(CanEverTick())
	{
		SetActorTickEnabled(State);
	}
	SetActorHiddenInGame(!State);
	SetActorEnableCollision(State);
}

void ABladeIdle::Activate()
{
	SetActive(true);
}

void ABladeIdle::StartActivationTimer()
{
	if (CanEverTick())
	{
		SetActorTickEnabled(true);
	}
	SetActorHiddenInGame(false);
	GetWorldTimerManager().SetTimer(ActivateTimer, this, &ABladeIdle::Activate, 0.5f, false);
}

void ABladeIdle::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(ASoulsCharacter* Char = Cast<ASoulsCharacter>(OtherActor))
	{
		Char->SetOverlappingItem(this);
	}
}

void ABladeIdle::Equip(USceneComponent* AttachTo, FName SocketName, FName SocketName2)
{
	if(AttachTo)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);

		if(BladeType == EBladeType::EBT_GreatSword)
		{
			// Spawn Greatsword et attacher le mesh au socket 1.
		}
		else if(BladeType == EBladeType::EBT_Dual)
		{
			// Spawn Dual blade et attacher les mesh aux deux sockets
		}
	}
}





