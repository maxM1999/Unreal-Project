// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;

/**
 * Classe qui sera transformé en BP pour ajuster les param du UI (bar, bouton, ect.)
 */
UCLASS()
class PROJETPRATIQUE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	// Associer cette progress bar à celle dans la classe BP (le nom doit matcher)
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
};
