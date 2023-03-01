// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SoulsCharacter.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/VirtualEyesComponent.h"

AEnemyAIController::AEnemyAIController() :
Player(nullptr),
Enemy(nullptr),
bShouldFaceTarget(false)
{
	EyesComponent = CreateDefaultSubobject<UVirtualEyesComponent>(TEXT("Eyes component"));
	if(EyesComponent)
	{
		EyesComponent->SetAttributes(90.f, 1000.f, 1200.f);
	}
}

void AEnemyAIController::BeginPlay() 
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetPawn());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	Player = Cast<ASoulsCharacter>(PlayerPawn);

	if (EyesComponent)
	{
		EyesComponent->SetOwnerEnemy(Enemy);
		EyesComponent->AddActorToPerceive(Player);
	}
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AEnemyAIController::SetMaxWalkSpeed(float InSpeed)
{
	if (Enemy && Enemy->GetCharacterMovement())
	{
		Enemy->GetCharacterMovement()->MaxWalkSpeed = InSpeed;
	}
}

void AEnemyAIController::SetOwnerUseControllerRotationYaw(bool InState) const
{
	if (Enemy)
	{
		Enemy->SetUseControllerRotationYaw(InState);
	}
}

void AEnemyAIController::GetPerceivedActors(TArray<AActor*>& InActors) const
{
	if (EyesComponent)
	{
		EyesComponent->GetPerceivedActors(InActors);
	}
}

bool AEnemyAIController::CheckEnemyOwner()
{
	if(!Enemy)
	{
		Enemy = Cast<AEnemy>(GetPawn());
	}

	if(Enemy)
	{
		return true;
	}
	return false;
}

void AEnemyAIController::ImpulseTowardTarget()
{
	if(!Player || !Enemy) return;

	const FRotator NewRotation = GetRotationTowardPlayer();
	Enemy->SetActorRotation(NewRotation);
	const FVector NewLoc = Enemy->GetActorLocation() + (Enemy->GetActorForwardVector() * 700.f);
	Enemy->SetActorLocation(NewLoc, true);
}

FRotator AEnemyAIController::GetRotationTowardPlayer() const
{
	FVector DirToTarget = (Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
	DirToTarget = FVector(DirToTarget.X, DirToTarget.Y, 0.f);
	return DirToTarget.Rotation();
}


