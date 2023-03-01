// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "HealthPotion.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::TakeDamage(float InDamage)
{
	if(InDamage > 0.f)
	{
		Health = FMath::Clamp(Health - InDamage, 0.f, MaxHealth);
	}
}

float UAttributeComponent::Heal(AActor* HealCauser)
{
	if (AHealthPotion* HealthPotion = Cast<AHealthPotion>(HealCauser))
	{
		const float HealAmount = HealthPotion->GetHealAmount();
		Health += HealAmount;
		Health = FMath::Clamp(Health, 0.f, MaxHealth);
		return Health;
	}

	return 0.0;
}

