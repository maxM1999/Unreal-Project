// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimNotify.h"
#include "Character/CharacterBase.h"


void UMyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(!Character && MeshComp)
	{
		AActor* OwningActor = MeshComp->GetOwner();
		Character = Cast<ACharacterBase>(OwningActor);
	}

	if(Character)
	{
		if(ImpulseDirection == "")
		{
			ImpulseDirection = FString("Forward");
		}
		Character->SetShouldAddImpulse(bShouldAddImpulse, ImpulseForce, ImpulseDirection);
	}
}
