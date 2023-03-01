// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/StateMachineBase.h"
#include "Kismet/GameplayStatics.h"
#include "States/StateBase.h"

void UStateMachineBase::Update()
{
	if(CurrentState && GetWorld())
	{
		const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
		CurrentState->Update(DeltaSeconds);
	}
}

void UStateMachineBase::StartStateMachineUpdate()
{
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UStateMachineBase::Update, 1.f / 60.f, true);
	}
}

void UStateMachineBase::InitStateMachine(AController* InOwnerController)
{
	OwnerController = InOwnerController;
}

void UStateMachineBase::InitStates()
{
}
