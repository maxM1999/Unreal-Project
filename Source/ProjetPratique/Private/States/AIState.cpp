// Fill out your copyright notice in the Description page of Project Settings.


#include "States/AIState.h"
#include "AI/EnemyAIController.h"

void UAIState::InitState( UAIStateMachine* InOwner, AEnemyAIController* InOwnerController)
{
	OwnerStateMachine = InOwner;
	OwnerController = InOwnerController;
}

void UAIState::Enter()
{
	Super::Enter();
}

void UAIState::Update(float DeltaSeconds)
{
	Super::Update(DeltaSeconds);
}

void UAIState::Exit()
{
	Super::Exit();
}

void UAIState::OnOwnerHit(AActor* Instigator)
{
}

void UAIState::OnOwnerHitEnd()
{
}
