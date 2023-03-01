// Fill out your copyright notice in the Description page of Project Settings.


#include "States/AICombatState.h"
#include "AI/EnemyAIController.h"
#include "Character/SoulsCharacter.h"
#include "Enemy/Enemy.h"

void UAICombatState::Enter()
{
	Super::Enter();

	if (OwnerController)
	{
		OwnerController->SetMaxWalkSpeed(600.f);
		
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAICombatState::RotateOwnerMeshTowardTarget, 0.5f, false);
		}
	}
}

void UAICombatState::Update(float DeltaSeconds)
{
	Super::Update(DeltaSeconds);

	if (OwnerController)
	{
		AActor* TargetToActor = Cast<AActor>(OwnerStateMachine->GetTarget());
		if (TargetToActor)
		{
			OwnerController->MoveToActor(TargetToActor, OwnerController->GetAcceptanceRadius());
		}
	}
}

void UAICombatState::Exit()
{
	Super::Exit();
}

void UAICombatState::RotateOwnerMeshTowardTarget()
{
	if (OwnerController && OwnerController->GetEnemyCharacter())
	{
		float DeltaSeconds = 0.f;

		if (GetWorld())
		{
			DeltaSeconds = GetWorld()->GetDeltaSeconds();
		}

		const FRotator CurrentOwnerMeshRot = OwnerController->GetEnemyCharacter()->GetActorRotation();
		const FRotator RotTowardTarget = OwnerController->GetRotationTowardPlayer();
		const FRotator NewRotation = FMath::RInterpTo(CurrentOwnerMeshRot, RotTowardTarget, DeltaSeconds, 15.f);
		OwnerController->GetEnemyCharacter()->SetActorRotation(RotTowardTarget);

		const FRotator CurrOwnerMeshRotation = OwnerController->GetEnemyCharacter()->GetActorRotation();
		/*if (CurrOwnerMeshRotation.Equals(NewRotation, 0.1f))
		{
			UE_LOG(LogTemp, Warning, TEXT("Finished rotating character"));
		}*/
	}
}
