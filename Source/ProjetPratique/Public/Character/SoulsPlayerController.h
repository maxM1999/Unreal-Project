// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "SoulsPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASoulsCharacter;
class UTargetingComponent;

enum class ERotationType : uint8
{
	ERT_PlayerDesiredRotation,
	ERT_ControlRotationYaw
};

UCLASS()
class PROJETPRATIQUE_API ASoulsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASoulsPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	void SetMeshTargetRotation(ERotationType RotationType);

private:
	void InitializePawnCharacter();
	void InitializeMappingContext() const;
	void SetupAnimationsWalkSpeed() const;
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void GamepadLookAround(const FInputActionValue& Value);
	void EquipWeapon() const;
	void LockOnTarget();
	void Attack() const;
	void Dodge() const;
	void Jump() const;
	void GetForwardAndRightVecFromController(FVector& OutFwd, FVector& OutRight) const;

	// Obtenir la rotation à laquelle le joueur doit s'orienter (en world space) selon son input (DesiredInputDirection). 
	FRotator GetPlayerDesiredRotation() const;
	
private:
	/** Character controllé par ce controller */
	UPROPERTY()
	ASoulsCharacter* SoulsCharacter;
	
	/* Inputs */
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAroundAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* GamepadLookAroundAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* EquipAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LockOnTargetAction;


	/* Movement */

	UPROPERTY(EditAnywhere)
	float TargetedWalkSpeed = 600.f;
	
	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0.0", UIMax = "1.0"), Category = "Input")
	float GamepadLookSensitivity = 1.f;

	// La direction (en world space) vers laquelle le joueur veut aller selon son Input.
	FVector2D DesiredInputDirection;

	// Est ce que le personnage doit rotationner de manière smooth cette frame-ci?
	bool bShouldRotateSmootly = false;

	// Si bShouldRotateSmootly est vrai, ceci est la rotation ciblée.
	FRotator TargetRotation;

	
	/* Targeting system */
	UPROPERTY(VisibleAnywhere, Category = "Target")
	UTargetingComponent* TargetingComponent;
	
	/* Type d'objet pouvant être targeté. À configurer dans le BP. */
	UPROPERTY(EditAnywhere, Category = "Target")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesThatCanBeTargeted;

public:
	FORCEINLINE ASoulsCharacter* GetSoulsCharacter() const {return SoulsCharacter;}
	FORCEINLINE void SetShouldRotateSmoothly(bool State) {bShouldRotateSmootly = State;}
	FORCEINLINE UTargetingComponent* GetTargetingComponent() const {return TargetingComponent;}
};




