// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthBarComponent.h"
#include "Components/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if(!HealthBarUserWidget)
	{
		HealthBarUserWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if(HealthBarUserWidget && HealthBarUserWidget->HealthBar)
	{
		HealthBarUserWidget->HealthBar->SetPercent(Percent);
	}
}

float UHealthBarComponent::GetHealthBarPercent()
{
	if(!HealthBarUserWidget)
	{
		HealthBarUserWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if(HealthBarUserWidget && HealthBarUserWidget->HealthBar)
	{
		return HealthBarUserWidget->HealthBar->Percent;
	}
	
	return 0.f;
}
