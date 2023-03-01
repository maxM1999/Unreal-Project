// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/AIState.h"
#include "AIPatrolState.generated.h"

enum class EPatrolState : uint8 
{
	/** L'AI est en train de se diriger vers sa destination */
	EPS_Moving,

	/** L'AI a atteint son point et attend pour se diriger vers le suivant */
	EPS_Waiting
};


UCLASS()
class PROJETPRATIQUE_API UAIPatrolState : public UAIState
{
	GENERATED_BODY()

private:
	void SetNewPath();

public:
	virtual void Enter() override;
	virtual void Update(float DeltaSeconds) override;
	virtual void Exit() override;
	virtual void OnOwnerHit(AActor* Instigator) override;
	virtual void OnOwnerHitEnd() override;

private:
	uint8 PatrolPointIndex;

	FTimerHandle ReachedDestinationTimer;

	EPatrolState PatrolState;

	FVector CurrPointToPatrol;

	/** Est ce qu'on doit vérifier si l'AI a atteint son point de navigation? */
	bool bShouldCheckPathStatus = true;

	bool bShouldMove = true;
};
