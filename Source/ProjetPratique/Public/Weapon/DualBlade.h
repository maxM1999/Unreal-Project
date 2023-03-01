// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blade.h"
#include "DualBlade.generated.h"

class AItem;
/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API ADualBlade final : public ABlade
{
	GENERATED_BODY()

public:
	ADualBlade();

private:
	/* Épées gauche */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* OtherMesh;
	
};
