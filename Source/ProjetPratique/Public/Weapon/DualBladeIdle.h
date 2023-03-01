// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BladeIdle.h"
#include "DualBladeIdle.generated.h"

/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API ADualBladeIdle : public ABladeIdle
{
	GENERATED_BODY()

public:
	ADualBladeIdle();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* OtherMesh;
	
};
