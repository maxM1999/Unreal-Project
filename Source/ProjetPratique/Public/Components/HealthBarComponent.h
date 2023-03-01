// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;

/**
 * Component qu'un actor peut attacher à son root component
 * et afficher un user widget.
 */
UCLASS()
class PROJETPRATIQUE_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);

	float GetHealthBarPercent();

private:
	UPROPERTY()
	UHealthBar* HealthBarUserWidget = nullptr;
	
};
