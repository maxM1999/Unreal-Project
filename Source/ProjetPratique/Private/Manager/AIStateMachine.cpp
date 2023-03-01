// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/AIStateMachine.h"
#include "AI/EnemyAIController.h"
#include "Character/SoulsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "States/AICombatState.h"
#include "States/AIPatrolState.h"
#include "States/StateBase.h"
#include "States/AIState.h"


void UAIStateMachine::ChangeState(EAIState NewState)
{
	if(CurrentState)
	{
		CurrentState->Exit();
	}
	
	CurrentState = *(States->Find(NewState));
	
	if(CurrentState)
	{
		CurrentState->Enter();
	}
}

void UAIStateMachine::SetPatrolPoints(const TArray<FVector>& InPatrolPoints)
{
	PatrolPoints = InPatrolPoints;
}

void UAIStateMachine::OnOwnerHit(AActor* Instigator)
{
	if (CurrentState)
	{
		if (UAIState* CurrAIState = Cast<UAIState>(CurrentState))
		{
			CurrAIState->OnOwnerHit(Instigator);
		}
	}
}

void UAIStateMachine::OnOwnerHitEnd()
{
	if (CurrentState)
	{
		if (UAIState* CurrAIState = Cast<UAIState>(CurrentState))
		{
			CurrAIState->OnOwnerHitEnd();
		}
	}
}

void UAIStateMachine::InitStates()
{
	Super::InitStates();

	UAIState* Patrol = NewObject<UAIPatrolState>(this, UAIPatrolState::StaticClass(), FName("AI Patrol State"));
	UAIState* Combat = NewObject<UAICombatState>(this, UAICombatState::StaticClass(), FName("AI Combat State"));
	
	States->Add(EAIState::EAS_Patrol, Patrol);
	States->Add(EAIState::EAS_Combat, Combat);
	
	TArray<EAIState> StatesKey;
	States->GetKeys(StatesKey);
	for(EAIState AIState : StatesKey)
	{
		UAIState* State = *(States->Find(AIState));
		if(State)
		{
			State->InitState(this, OwnerAiController);
		}
	}
	
	ChangeState(EAIState::EAS_Patrol);
}

void UAIStateMachine::InitStateMachine(AController* InOwnerController)
{
	Super::InitStateMachine(InOwnerController);

	States = new TMap<EAIState, UAIState*>();

	if(InOwnerController)
	{
		OwnerAiController = Cast<AEnemyAIController>(InOwnerController);
		Target = Cast<ASoulsCharacter>(UGameplayStatics::GetPlayerCharacter(OwnerAiController, 0));
	}

	InitStates();
}
