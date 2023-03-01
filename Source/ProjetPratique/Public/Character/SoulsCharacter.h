// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "SoulsCharacter.generated.h"

class ASoulsPlayerController;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UTargetingComponent;
class ABladeIdle;
class AWeaponManager;

UCLASS()
class PROJETPRATIQUE_API ASoulsCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	ASoulsCharacter();
	virtual void Tick(float DeltaTime) override;
	void SetOrientCharacterToMovement(bool State);
	virtual void Attack() override;
	virtual void Dodge() override;
	bool IsCharacterArmed() const;

	// Hérité via IHitInterface
	virtual void GetHit(const FVector& ImpactPoint) override;

	/** Équiper l'arme dans le monde. */
	virtual void EquipWeapon(EBladeType TypeToEquip) override;

	/** Changer l'arme couramment utilisée avec celle dans le monde */
	void ChangeWeapon();

private:
	void UpdateComboIndex();

	/** Gérer la rotation du mesh selon le targeting state. */
	void UpdateMeshRotation();

	/** Selon l'arme équipé, jouer le bon animation montage */
	void PlayCorrespondingAttackMontage();
		
	/** Selon l'arme équipée, retourne le montage à jouer. */
	UAnimMontage* GetCorrespondingMontageToPlay() const;

	/** Selon l'arme équipée, jouer le bon animation montage */
	void PlayCorrespondingDodgeMontage();

	UFUNCTION(BlueprintCallable)
	void AddImpulse();

protected:
	virtual void BeginPlay() override;
	virtual void PlayAttackMontageSection(UAnimMontage* MontageToPlay, FName Section) override;
	virtual void MakeCharacterJump() override;
	virtual void AttackEnd(uint8 AttackNumber, bool bIsLastNotify, bool bIsFromLastSection) override;

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	/* Dernière chance pour le joueur de réorienter son attaque. */
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void LateReOrient();

private:
	UPROPERTY()
	ASoulsPlayerController* SoulsPlayerController;

	/** Si > 0, le joueur veut réattaquer. */
	UPROPERTY(VisibleAnywhere, Category="Combat")
	uint8 ComboIndex = 0;
	
	/* Components */

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	/* arme avec laquelle le joueur overlap */
	UPROPERTY(VisibleAnywhere)
	ABladeIdle* OverlappingBlade;
	

	/* States */

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unocupied;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ETargetState TargetState = ETargetState::ETS_NonTargeted;

	/* Animations */

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* GreatswordDodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* GreatswordAttackMontage;

	/** Utiliser pour équiper des déséquiper des armes. */
	AWeaponManager* WeaponManager;

public:
	FORCEINLINE void SetOverlappingItem(ABladeIdle* Item) { OverlappingBlade = Item; }
	FORCEINLINE AController* GetController() const { return Controller; }
	FORCEINLINE EEquipementState GetEquipementState() const {return EquipementState;}
	FORCEINLINE void SetEquipementState(const EEquipementState State) {EquipementState = State;}
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
	FORCEINLINE void SetActionState(const EActionState State) {ActionState = State;}
	FORCEINLINE ABladeIdle* GetOverlappingBlade() const {return OverlappingBlade;}
	FORCEINLINE ABlade* GetBladeLeftHand() const {return BladeLeftHand;}
	FORCEINLINE ABlade* GetBladeRightHand() const {return BladeRightHand;}
	FORCEINLINE void SetBladeLeftHand(ABlade* InBlade) {BladeLeftHand = InBlade;}
	FORCEINLINE void SetBladeRightHand(ABlade* InBlade) {BladeRightHand = InBlade;}
	FORCEINLINE ETargetState GetTargetState() const {return TargetState;}
	FORCEINLINE void SetTargetState(const ETargetState NewState) {TargetState = NewState;}

	UFUNCTION(BlueprintCallable, Category = "Character States")
	FORCEINLINE void SetCharacterActionState(EActionState NewState) { ActionState = NewState; }
	
};
