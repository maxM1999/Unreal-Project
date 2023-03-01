// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Weapon/BladeTypes.h"
#include "CharacterBase.generated.h"

class UCharacterAnimInstanceBase;
class UAttributeComponent;
class ABlade;

UCLASS()
class PROJETPRATIQUE_API ACharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	float GetMaxWalkSpeed() const;
	virtual void EquipWeapon(EBladeType TypeToEquip);
	virtual void Attack();
	virtual void Dodge();

	// Hérité via IHitInterface
	virtual void GetHit(const FVector& ImpactPoint) override;

	UFUNCTION(BlueprintCallable)
	void SetShouldAddImpulse(bool State, float ImpulseAmount, FString Direction);

	virtual void OnHealthPotionPickedUp(AActor* HealCauser);
	
protected:
	virtual void BeginPlay() override;
	virtual void PlayAttackMontageSection(UAnimMontage* MontageToPlay, FName Section);
	virtual void MakeCharacterJump();

	/** Joue le bon Hit montage selon le point d'impact */
	void HandleDirectionalHit(const FVector& ImpactPoint);
	void PlayHitMontageFromAngle(const double& Angle);
	void PlatHitMontage(const FName& Section, UAnimMontage* ToPlay);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	/** 
	* @param AttackNumber numéro de la section qui vient de finir
	* @param bIsLastNotify  Indique si cette notification est la dernière de sa section.
	* @param bIsFromLastSection Indique si cette attaque vient de la dernière section.
	*/
	virtual void AttackEnd(uint8 AttackNumber, bool bIsLastNotify, bool bIsFromLastSection);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABlade* BladeLeftHand = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABlade* BladeRightHand = nullptr;

	/* Animation */
	
	UPROPERTY()
	UCharacterAnimInstanceBase* CharacterAnimInstance;

	/** Montage pour les dual blades */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HitMontageGreat;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DodgeMontage;

	/* Movement */

	UPROPERTY()
	UCharacterMovementComponent* MovementComp;

	/* Components */
	
	/** Component qui gère la vie, l'endurance, ect. */
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

	/* VFX */
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* HitParticles;


	/** Impulse for animations */

	bool bShouldAddImpulse = false;
	float ImpulseForce = 10000.f;
	FString ImpulseDirection;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), VisibleAnywhere)
	EEquipementState EquipementState = EEquipementState::ECS_Unequipped;
};
