// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJETPRATIQUE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TakeDamage(float InDamage);

	/** @param Heal: Actor responsable du heal */
	float Heal(AActor* HealCauser);

protected:
	virtual void BeginPlay() override;

private:
	// Vie courante de l'actor.
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

public:
	FORCEINLINE float GetHealth() const {return Health;}
	FORCEINLINE float GetHealthPercent() const {return Health / MaxHealth;}
};
