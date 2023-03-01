// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VirtualEyesComponent.h"
#include "Enemy/Enemy.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UVirtualEyesComponent::UVirtualEyesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UVirtualEyesComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UVirtualEyesComponent::SetView()
{
	const FVector RayStart = OwnerEnemy->GetActorLocation();
	const FVector Forward = OwnerEnemy->GetActorForwardVector();
	const FVector Right = OwnerEnemy->GetActorRightVector();

	// Trouver la longueur de chaque raycast avec la trigo
	float CornerAngle = 180.f - 90.f - (SightAngle / 2);
	float CornerAngleRad = FMath::DegreesToRadians(CornerAngle);
	float RayLength = MaxSightDistance / FMath::Sin(CornerAngleRad);

	// pythagore pour trouver la moitié du côté face au joueur
	float FrontHalf = FMath::Sqrt((RayLength * RayLength) - (MaxSightDistance * MaxSightDistance));

	FVector LeftRayEnd = RayStart + (Forward * MaxSightDistance) - (Right * FrontHalf);
	FVector RightRayEnd = RayStart + (Forward * MaxSightDistance) + (Right * FrontHalf);

	DrawDebugLine(GetWorld(), RayStart, LeftRayEnd, FColor::Red, false, -1);
	DrawDebugLine(GetWorld(), RayStart, RightRayEnd, FColor::Red, false, -1);
	DrawDebugLine(GetWorld(), LeftRayEnd, RightRayEnd, FColor::Red, false, -1);
}

void UVirtualEyesComponent::GetPerceivedActors(TArray<AActor*>& InActors)
{
	for (AActor* Actor : ActorsToPerceive)
	{
		const FVector ActorLoc = Actor->GetActorLocation();
		const FVector AILoc = OwnerEnemy->GetActorLocation();
		
		if (IsViewBlockByObstacle(AILoc, ActorLoc, Actor))
		{
			continue;
		}

		const float DistBetweenTwoActors = FVector::Dist(ActorLoc, AILoc);
		const FVector DirBetweenTwoLoc = (ActorLoc - AILoc).GetSafeNormal();
		const FRotator DirectionWorldRotation = DirBetweenTwoLoc.Rotation();
		const FVector AIForward = OwnerEnemy->GetActorForwardVector();
		const float RadAngleBetweenForwardAndDir = FMath::Acos(AIForward.Dot(DirBetweenTwoLoc));
		const float DegAngleBetweenForwardAndDir = FMath::RadiansToDegrees(RadAngleBetweenForwardAndDir);

		if (DegAngleBetweenForwardAndDir <= SightAngle / 2)
		{
			// selon l'angle, est ce que l'actor est a porté d'etre vu?
			const float NewDist = MaxSightDistance / FMath::Cos(RadAngleBetweenForwardAndDir);
			if (DistBetweenTwoActors <= NewDist)
			{
				InActors.Add(Actor);
			}
		}
	}
}

bool UVirtualEyesComponent::IsViewBlockByObstacle(const FVector& ViewStart, const FVector& ViewEnd, const AActor* Target)
{
	if (!OwnerEnemy || !Target) return true;

	TArray<AActor*> ToIgnore;
	ToIgnore.Add(OwnerEnemy);
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), ViewStart, ViewEnd, ETraceTypeQuery::TraceTypeQuery1, false, ToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor != Target)
		{
			return true;
		}
	}

	return false;
}

// Called every frame
void UVirtualEyesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetView();

	TArray<AActor*> PerceivedActors;
	GetPerceivedActors(PerceivedActors);

	for (AActor* Actor : PerceivedActors)
	{
		DrawDebugLine(GetWorld(), OwnerEnemy->GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, -1, 0, 2.f);
		DrawDebugSphere(GetWorld(), Actor->GetActorLocation(), 24, 12, FColor::Green, false, -1);
	}
	
}

AEnemy* UVirtualEyesComponent::GetOwnerEnemy() const
{
	return OwnerEnemy;
}

void UVirtualEyesComponent::SetAttributes(float InSightAngle, float InMaxSightDistance, float InLoseSightDistance)
{
	SightAngle = InSightAngle;
	MaxSightDistance = InMaxSightDistance;
	LoseSightDistance = InLoseSightDistance;
}

void UVirtualEyesComponent::SetOwnerEnemy(AEnemy* InEnemy)
{
	OwnerEnemy = InEnemy;
}

void UVirtualEyesComponent::AddActorToPerceive(AActor* NewActor)
{
	ActorsToPerceive.Add(NewActor);
}

