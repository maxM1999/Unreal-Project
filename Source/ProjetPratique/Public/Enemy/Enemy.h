// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Enemy.generated.h"

class UAIStateMachine;
class UHealthBarComponent;

UCLASS()
class PROJETPRATIQUE_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit(const FVector& ImpactPoint) override;
	void SetMaxWalkSpeed(const float& NewSpeed) const;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attack() override;
	void SetUseControllerRotationYaw(bool InState);
	
protected:
	virtual void BeginPlay() override;
	virtual void AttackEnd(uint8 AttackNumber, bool bIsLastNotify, bool bIsFromLastSection) override;

	UFUNCTION(BlueprintCallable)
	void OnHitEnd();

private:
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere)
	TArray<FVector> PatrolPoints;

	UPROPERTY()
	UAIStateMachine* StateMachine;
};
