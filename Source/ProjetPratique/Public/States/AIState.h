// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/AIStateMachine.h"
#include "States/StateBase.h"
#include "AIState.generated.h"

class AEnemyAIController;
class UAIStateMachine;
/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API UAIState : public UStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UAIStateMachine* OwnerStateMachine;

	UPROPERTY()
	AEnemyAIController* OwnerController;

public:
	void InitState(UAIStateMachine* InOwner, AEnemyAIController* InOwnerController);
	virtual void Enter() override;
	virtual void Update(float DeltaSeconds) override;
	virtual void Exit() override;
	virtual void OnOwnerHit(AActor* Instigator);
	virtual void OnOwnerHitEnd();
};
