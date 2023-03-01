// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class ASoulsPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJETPRATIQUE_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Distance à laquelle celui qui possède ce component peut target */
	UPROPERTY(EditAnywhere)
	float TargetingRange = 1000.f;

	UPROPERTY(EditAnywhere)
	float SphereRadius = 25.f;

	/** Angle vertical de la caméra lorsque le joueur target un ennemi. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "-45"), meta = (ClampMax = "0"))
	float CameraPitchRotationWhenLockedOn = -20.f;

	UPROPERTY()
	ASoulsPlayerController* Owner;

	UPROPERTY()
	AActor* TargetedActor;

	/** Rotationner la caméra de manière smooth cette frame - ci ?
	*   True dès les premiers moment que le joueur target un ennemi valide. Évite le snapping. */
	bool bShouldRotateOwnerControllerSmoothly = false;

	/* Rotation à laquelle le controller va changer sa rotation pour essayer de l'égaler. */
	FRotator ControlTargetRotation;

public:	
	UTargetingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** @param ObjectType: Type d'objet pouvant être verouillé par le sphere trace. */
	AActor* TryTargetActor(TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectType);

protected:
	virtual void BeginPlay() override;

private:
	/** Retourne la rotation que le controller de l'owner doit avoir pour locker la cible. 
	*  Utilisé pour faire une transition smooth.
	*  Le pitch à une valeur définit selon les préférences du joueur et le Roll égal 0.
	*/
	FRotator GetTargetRotation() const;

	bool PerformSphereTrace(FHitResult& OutHit, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectType);

	bool IsOwnerControlRotationNearlyEqualTo(const FRotator& OtherRotator, double Tolerance) const;

	/** Affecte le mesh et le controller de l'owner. */
	void ShouldRotateOwnerComponents(bool State);

	/** Retourne vrai si la rotation du controller de l'owner pointe parfaitement sur la target. */
	bool SmoothlyRotateOwnerControllerTowardTarget(float DeltaTime) const;

	/** Rotationne le controller de l'owner de manière à ce qu'il pointe parfaitement sur la target.
	* La rotation ne se fait PAS de manière smooth.
	*/
	void RotateOwnerControllerTowardTarget() const;

public:
	FORCEINLINE void SetTargetingRange(const float& Value) { TargetingRange = Value; }
	FORCEINLINE void SetSphereRadius(const float& Value) { SphereRadius = Value; }
	FORCEINLINE void SetOwner(ASoulsPlayerController* NewOwner) { Owner = NewOwner; }
	FORCEINLINE bool HasTarget() const { return TargetedActor ? true : false; }
};
