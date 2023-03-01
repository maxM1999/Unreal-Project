// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateMachineBase.generated.h"

class UStateBase;

/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API UStateMachineBase : public UObject
{
	GENERATED_BODY()

protected:
	/** State courante dans laquelle la state machine se trouve */
	UPROPERTY()
	UStateBase* CurrentState;

	/** Timer handle pour gérer l'appel de Update */ 
	FTimerHandle TimerHandle;

	/** Controller du Pawn qui est associé à cette state machine */
	UPROPERTY()
	AController* OwnerController;

public:
	void Update();

	/** Fait en sorte que la méthode Update est appelée chaque frames (60/sec) */
	void StartStateMachineUpdate();

	/** Remplace le constructeur */
	virtual void InitStateMachine(AController* InOwnerController);

	/** Instancie toute les states nécessaires pour la state machine */
	virtual void InitStates();

public:
	/** Retourne le controller du Pawn qui possède cette state machine */
	FORCEINLINE AController* GetOwnerController() const {return OwnerController;}
};
