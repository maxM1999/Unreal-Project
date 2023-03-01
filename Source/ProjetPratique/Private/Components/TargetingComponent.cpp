// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Character/SoulsCharacter.h"
#include "Character/SoulsPlayerController.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (TargetedActor && Owner)
	{
		if (bShouldRotateOwnerControllerSmoothly)
		{
			// Si le joueur a bougé avant que la rotation de la caméra soit = TargetRotation (pour éviter le snapping)
			// Interpoler tant que la rotation de la caméra n'est pas = TargetRotation.
			if (ControlTargetRotation != GetTargetRotation())
			{
				ControlTargetRotation = GetTargetRotation();
			}

			if (SmoothlyRotateOwnerControllerTowardTarget(DeltaTime))
			{
				bShouldRotateOwnerControllerSmoothly = false;
			}
		}
		else
		{
			RotateOwnerControllerTowardTarget();
		}
	}
}

FRotator UTargetingComponent::GetTargetRotation() const
{
	if (Owner && TargetedActor)
	{
		FVector DirTowardTarget = TargetedActor->GetActorLocation() - Owner->GetSoulsCharacter()->GetActorLocation();
		DirTowardTarget = DirTowardTarget.GetSafeNormal();
		return FRotator(CameraPitchRotationWhenLockedOn, DirTowardTarget.Rotation().Yaw, 0.f);
	}
	return FRotator::ZeroRotator;
}

bool UTargetingComponent::PerformSphereTrace(FHitResult& OutHit, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectType)
{
	if (Owner)
	{
		FRotator OwnerControlRotation = Owner->GetControlRotation();
		OwnerControlRotation = FRotator(0, OwnerControlRotation.Yaw, 0.f);
		const FVector OwnerControllerForward = FRotationMatrix(OwnerControlRotation).GetUnitAxis(EAxis::X);
		const FVector Start = Owner->GetSoulsCharacter()->GetActorLocation();
		const FVector End = Start + (OwnerControllerForward * TargetingRange);

		TArray<AActor*> ToIgnore;
		ToIgnore.Add(Owner->GetSoulsCharacter());

		return UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, SphereRadius, ObjectType, false, ToIgnore, EDrawDebugTrace::None, OutHit, true);
	}
	return false;
}

bool UTargetingComponent::IsOwnerControlRotationNearlyEqualTo(const FRotator& OtherRotator, double Tolerance) const
{
	if (Owner)
	{
		const FRotator CurrentOwnerControlRotation = Owner->GetControlRotation();
		return CurrentOwnerControlRotation.Equals(OtherRotator, Tolerance);
	}
	return false;
}

void UTargetingComponent::ShouldRotateOwnerComponents(bool State)
{
	bShouldRotateOwnerControllerSmoothly = State;
	
	if (Owner->GetSoulsCharacter())
	{
		Owner->GetSoulsCharacter()->SetOrientCharacterToMovement(!State);
	}
}

bool UTargetingComponent::SmoothlyRotateOwnerControllerTowardTarget(float DeltaTime) const
{
	const FRotator CurrentOwnerControlRotation = Owner->GetControlRotation();
	const FRotator RotationThisFrame = FMath::RInterpTo(CurrentOwnerControlRotation, ControlTargetRotation, DeltaTime, 20.f);
	Owner->SetControlRotation(RotationThisFrame);

	return IsOwnerControlRotationNearlyEqualTo(ControlTargetRotation, 0.1f);
}

void UTargetingComponent::RotateOwnerControllerTowardTarget() const
{
	const FVector DirToTarget = TargetedActor->GetActorLocation() - Owner->GetSoulsCharacter()->GetActorLocation();
	const FRotator Rot = FRotator(-20.f, DirToTarget.Rotation().Yaw, 0.f);
	Owner->SetControlRotation(Rot);
}

AActor* UTargetingComponent::TryTargetActor(TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectType)
{
	if (!TargetedActor)
	{
		FHitResult HitResult;
		PerformSphereTrace(HitResult, ObjectType);

		if (HitResult.bBlockingHit && HitResult.GetActor())
		{
			ShouldRotateOwnerComponents(true);

			TargetedActor = HitResult.GetActor();
			ControlTargetRotation = GetTargetRotation();
			
			return HitResult.GetActor();
		}
		return nullptr;
	}
	else // unlock la cible.
	{
		ShouldRotateOwnerComponents(false);
		TargetedActor = nullptr;
		return nullptr;
	}
}



