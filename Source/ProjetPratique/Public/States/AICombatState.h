// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/AIState.h"
#include "AICombatState.generated.h"

/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API UAICombatState : public UAIState
{
	GENERATED_BODY()

private:
	FTimerHandle TimerHandle;

public:
	virtual void Enter() override;
	virtual void Update(float DeltaSeconds) override;
	virtual void Exit() override;

	void RotateOwnerMeshTowardTarget();
};
