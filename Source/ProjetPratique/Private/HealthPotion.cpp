// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "Character/SoulsCharacter.h"

void AHealthPotion::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (ASoulsCharacter* Char = Cast<ASoulsCharacter>(OtherActor))
	{
		Char->OnHealthPotionPickedUp(this);
	}

	Destroy();
}

float AHealthPotion::GetHealAmount() const
{
	return HealAmount;
}
