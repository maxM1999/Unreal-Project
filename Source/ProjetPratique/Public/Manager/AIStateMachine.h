// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/StateMachineBase.h"
#include "Character/CharacterStates.h"
#include "AIStateMachine.generated.h"

class AEnemyAIController;
class ASoulsCharacter;
class UAIState;
/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API UAIStateMachine : public UStateMachineBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AEnemyAIController* OwnerAiController;

	TMap<EAIState, UAIState*>* States;

	/** Dans le cas d'un AI, Target est le player */
	UPROPERTY()
	ASoulsCharacter* Target;

	/** Points entre lesquelles l'AI se promenne lorsqu'il est dans l'état Patrol */
	TArray<FVector> PatrolPoints;

public:
	virtual void InitStateMachine(AController* InOwnerController) override;
	virtual void InitStates() override;
	void ChangeState(EAIState NewState);
	void SetPatrolPoints(const TArray<FVector>& InPatrolPoints);
	void OnOwnerHit(AActor* Instigator);
	void OnOwnerHitEnd();
	
	FORCEINLINE TArray<FVector> GetPatrolPoints() const {return PatrolPoints;}
	FORCEINLINE AEnemyAIController* GetAIController() const {return OwnerAiController;}
	FORCEINLINE ASoulsCharacter* GetTarget() const { return Target; }
};
