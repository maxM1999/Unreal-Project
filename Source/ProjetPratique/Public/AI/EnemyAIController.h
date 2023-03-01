// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/CharacterStates.h"
#include "EnemyAIController.generated.h"

class AEnemy;
class ASoulsCharacter;
class UVirtualEyesComponent;

UCLASS()
class PROJETPRATIQUE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	/** Référence vers le joueur */
	UPROPERTY()
	ASoulsCharacter* Player;

	/** Référence vers l'enemi qui possède ce controller */
	UPROPERTY()
	AEnemy* Enemy;

	FTimerHandle Timer;
	bool bShouldFaceTarget;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed;

	/** Lorsque l'AI poursuit une cible, a quelle distance il s'arrête? */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptanceRadiusToTarget = 200.f;

	/** Component simulant le sens de la vue */
	UPROPERTY(VisibleAnywhere)
	UVirtualEyesComponent* EyesComponent;

protected:
	virtual void BeginPlay() override;

private:
	bool CheckEnemyOwner();
	void ImpulseTowardTarget();
	
public:
	AEnemyAIController();
	virtual void Tick(float DeltaSeconds) override;
	void SetMaxWalkSpeed(float InSpeed);
	void SetOwnerUseControllerRotationYaw(bool InState) const;
	void GetPerceivedActors(TArray<AActor*>& InActors) const;
	FRotator GetRotationTowardPlayer() const;
	
	FORCEINLINE AEnemy* GetEnemyCharacter() const {return Enemy;}
	FORCEINLINE float GetWalkSpeed() const { return WalkSpeed; }
	FORCEINLINE float GetAcceptanceRadius() const { return AcceptanceRadiusToTarget; }
};
