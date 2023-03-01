// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Blade.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/SoulsCharacter.h"
#include "Interfaces/HitInterface.h"

ABlade::ABlade()
{
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MainMesh);

	BladeStart = CreateDefaultSubobject<USceneComponent>(TEXT("BladeStartPoint"));
	BladeStart->SetupAttachment(GetRootComponent());
	BladeEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BladeEndPoint"));
	BladeEnd->SetupAttachment(GetRootComponent());

	OwnerCharacter = nullptr;

	SetActive(false);
}

void ABlade::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlade::SetShouldLineTrace(bool State)
{
	bShouldLineTrace = State;
	ClearActorsToIgnoreList();
}

void ABlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldLineTrace)
	{
		const FVector LineTraceStart = BladeStart->GetComponentLocation();
		const FVector LineTraceEnd = BladeEnd->GetComponentLocation();
		TArray<AActor*> ToIgnore;

		for (AActor* Actor : ActorsToIgnore)
		{
			ToIgnore.AddUnique(Actor);
		}

		FHitResult LineTraceHitResult;

		UKismetSystemLibrary::LineTraceSingle(
			this,
			LineTraceStart,
			LineTraceEnd,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ToIgnore,
			EDrawDebugTrace::None,
			LineTraceHitResult,
			true
		);

		AActor* HitActor = LineTraceHitResult.GetActor();
		
		if (IHitInterface* HitInterface = Cast<IHitInterface>(HitActor))
		{
			if (GetOwner())
			{
				HitInterface->GetHit(GetOwner()->GetActorLocation());
			}
		}

		if(HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
		ActorsToIgnore.AddUnique(HitActor);
	}
}

void ABlade::Equip(USceneComponent* CompToAttachTo, FName SocketName)
{
	SetActive(true);
	
	if(MainMesh)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
		MainMesh->AttachToComponent(CompToAttachTo, Rules, SocketName);
	}
}

void ABlade::DeEquip(FVector& ReplaceLocation)
{
	SetOwner(nullptr);
	SetInstigator(nullptr);
	SetActive(false);
}

void ABlade::SetActive(bool State)
{
	if (CanEverTick())
	{
		SetActorTickEnabled(State);
	}

	SetActorHiddenInGame(!State);
	SetActorEnableCollision(State);
}

void ABlade::ClearActorsToIgnoreList()
{
	ActorsToIgnore.Empty();
}

