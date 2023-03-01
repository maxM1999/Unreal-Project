// Fill out your copyright notice in the Description page of Project Settings.


#include "States/AIPatrolState.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI/EnemyAIController.h"
#include "Character/SoulsCharacter.h"
#include "Enemy/Enemy.h"


void UAIPatrolState::Enter()
{
	Super::Enter();

	PatrolPointIndex = 0;
	PatrolState = EPatrolState::EPS_Moving;

	if(OwnerController && OwnerController->GetPawn() && OwnerStateMachine)
	{
		OwnerController->SetOwnerUseControllerRotationYaw(false);
		OwnerController->SetMaxWalkSpeed(OwnerController->GetWalkSpeed());
		CurrPointToPatrol = OwnerStateMachine->GetPatrolPoints()[0];
		OwnerController->MoveToLocation(CurrPointToPatrol);
	}
}

void UAIPatrolState::Update(float DeltaSeconds)
{
	Super::Update(DeltaSeconds);

	UPathFollowingComponent* PathFollowingComponent = OwnerController->GetPathFollowingComponent();
	if (bShouldCheckPathStatus && PathFollowingComponent->DidMoveReachGoal() && GetWorld())
	{
		bShouldCheckPathStatus = false;
		PatrolState = EPatrolState::EPS_Waiting;
		GetWorld()->GetTimerManager().SetTimer(ReachedDestinationTimer, this, &UAIPatrolState::SetNewPath, 2.5f, false);
	}

	if (PathFollowingComponent)
	{
		if (!bShouldMove)
		{
			if (PathFollowingComponent->GetStatus() == EPathFollowingStatus::Moving)
			{
				PathFollowingComponent->PauseMove();
			}
		}
		else
		{
			if (PathFollowingComponent->GetStatus() == EPathFollowingStatus::Paused)
			{
				PathFollowingComponent->ResumeMove();
			}
			else if (PathFollowingComponent->GetStatus() == EPathFollowingStatus::Idle)
			{
				if (OwnerController)
				{
					OwnerController->MoveToLocation(CurrPointToPatrol);
				}
			}
		}
	}
	

	//if (OwnerController)
	//{
	//	TArray<AActor*> PerceivedActors;
	//	OwnerController->GetPerceivedActors(PerceivedActors);

	//	for (AActor* Actor : PerceivedActors)
	//	{
	//		// L'AI voit le joueur
	//		if (Actor == OwnerStateMachine->GetTarget())
	//		{
	//			OwnerStateMachine->ChangeState(EAIState::EAS_Combat);
	//		}
	//	}
	//}
}

void UAIPatrolState::Exit()
{
	Super::Exit();

}

void UAIPatrolState::SetNewPath()
{
	bShouldCheckPathStatus = true;
	PatrolState = EPatrolState::EPS_Moving;

	const uint8 PatrolPointCnt = OwnerStateMachine->GetPatrolPoints().Num();
	PatrolPointIndex++;
	if (PatrolPointIndex == PatrolPointCnt)
	{
		PatrolPointIndex = 0;
	}
	CurrPointToPatrol = OwnerStateMachine->GetPatrolPoints()[PatrolPointIndex];

	OwnerController->MoveToLocation(CurrPointToPatrol);
}

void UAIPatrolState::OnOwnerHit(AActor* Instigator)
{
	bShouldMove = false;
	bShouldCheckPathStatus = false;
}

void UAIPatrolState::OnOwnerHitEnd()
{
	bShouldMove = true;
	bShouldCheckPathStatus = true;
}
