// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharacterAnimInstanceBase.h"

#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!Character)
	{
		Character = Cast<ACharacterBase>(TryGetPawnOwner());
	}
}

void UCharacterAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character)
	{
		Character = Cast<ACharacterBase>(TryGetPawnOwner());
	}

	if (Character && Character->GetMovementComponent())
	{
		if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent()))
		{
			bIsOnGround = !MovementComponent->IsFalling();
			const FVector PlayerVelocity = MovementComponent->Velocity;
			const FVector PlayerLateralVelocity = FVector(PlayerVelocity.X, PlayerVelocity.Y, 0.f);
			GroundSpeed = PlayerLateralVelocity.Length();
		}
	}
}
