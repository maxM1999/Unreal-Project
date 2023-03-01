// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API UStateBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Enter();
	virtual void Update(float DeltaSeconds);
	virtual void Exit();
};
