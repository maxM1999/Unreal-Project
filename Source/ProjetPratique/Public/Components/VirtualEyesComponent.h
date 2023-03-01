// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VirtualEyesComponent.generated.h"

class AEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJETPRATIQUE_API UVirtualEyesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVirtualEyesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	AEnemy* GetOwnerEnemy() const;
	void SetAttributes(float InSightAngle, float InMaxSightDistance, float InLoseSightDistance);
	void SetOwnerEnemy(AEnemy* InEnemy);
	void AddActorToPerceive(AActor* NewActor);
	void GetPerceivedActors(TArray<AActor*>& InActors);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float SightAngle = 90.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxSightDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseSightDistance = 1200.f;

	TArray<AActor*> ActorsToPerceive;

	UPROPERTY()
	AEnemy* OwnerEnemy;

private:
	/** Update les raycast de la zone de vue */
	void SetView();

	/** @param ViewStart: Position de d�but de la vue du AI
	* @param ViewEnd: Position de l'actor qui peut �tre vu par l'AI
	* @param Target: Actor qui peut �tre vu
	*/
	bool IsViewBlockByObstacle(const FVector& ViewStart, const FVector& ViewEnd, const AActor* Target);
};
