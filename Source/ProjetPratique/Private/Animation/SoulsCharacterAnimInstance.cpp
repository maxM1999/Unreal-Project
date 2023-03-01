// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SoulsCharacterAnimInstance.h"
#include "Character/CharacterBase.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLIbrary.h"

void USoulsCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USoulsCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Character && Character->GetController() && Character->GetMovementComponent())
	{
		// Rotation en world space du point de vue du joueur.
		FRotator AimRotation = Character->GetBaseAimRotation();
		// Rotation en world space par rapport à l'axis X de la direction du mouvent du joueur.
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	}
}

