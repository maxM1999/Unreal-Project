// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "Animation/CharacterAnimInstanceBase.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Blade.h"



ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	ImpulseDirection = FString("None");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetMesh())
	{
		CharacterAnimInstance = Cast<UCharacterAnimInstanceBase>(GetMesh()->GetAnimInstance());
	}

	if (UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComp = CharacterMovementComponent;
		MovementComp->bOrientRotationToMovement = true;
	}
}

void ACharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bShouldAddImpulse)
	{
		FVector DirectionToApplyImpulse = FVector::ZeroVector;

		if(ImpulseDirection.Equals(TEXT("Left")))
		{
			DirectionToApplyImpulse = DirectionToApplyImpulse = -GetActorRightVector();
		}
		else if(ImpulseDirection.Equals(TEXT("Right")))
		{
			DirectionToApplyImpulse = DirectionToApplyImpulse = GetActorRightVector();
		}
		else if(ImpulseDirection.Equals(TEXT("Back")))
		{
			DirectionToApplyImpulse = -GetActorForwardVector();
		}
		else
		{
			DirectionToApplyImpulse = GetActorForwardVector();
		}

		if(GetCharacterMovement())
		{
			GetCharacterMovement()->AddImpulse(DirectionToApplyImpulse * ImpulseForce);
		}
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float ReturnedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(AttributeComponent && ReturnedDamage > 0.f)
	{
		AttributeComponent->TakeDamage(ReturnedDamage);
	}

	return ReturnedDamage;
}

void ACharacterBase::HandleDirectionalHit(const FVector& ImpactPoint)
{
	/**
	* Déterminer si coup vient de quelle direction.
	* Récupérer le forward de l'actor qui recoit le coup et obtenir l'angle
	* entre le fwd et le point d'impact en faisant le dot product.
	* Faire ensuite le cross product entre ces deux même vectors pour obtenir
	* un vector perpendiculaire. Unreal utilise la main gauche. Ainsi, si le vector
	* résultant pointe vers le bas, le coup vient de la gauche. On doit donc changer le signe
	* de l'angle.
	*/

	const FVector Forward = GetActorForwardVector();

	// Descendre le point d'impact à la même hauteur que la position de l'enemie.
	const FVector ImpactPointLower = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

	// Vector unitaire partant de la position de l'actor et se terminant au point d'impact.
	const FVector ToHit = (ImpactPointLower - GetActorLocation()).GetSafeNormal();

	// retourne cos(angle) entre les deux vectors.
	const double CosAngle = FVector::DotProduct(Forward, ToHit);

	// Angle entre les deux vectors.
	double Angle = FMath::Acos(CosAngle);

	// Convertir l'angle en deg.
	Angle = FMath::RadiansToDegrees(Angle);

	// Si pointe vers le bas, l'angle est négatif.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Angle = -Angle;
	}

	PlayHitMontageFromAngle(Angle);
}

void ACharacterBase::PlayHitMontageFromAngle(const double& Angle)
{
	UAnimMontage* ToPlay = nullptr;
	switch(EquipementState)
	{
	case EEquipementState::ECS_Equipped_Dual:
		ToPlay = HitMontage;
		break;
	case EEquipementState::ECS_Equipped_Greatsword:
		ToPlay = HitMontageGreat;
		break;
	case EEquipementState::ECS_Unequipped:
		break;
	}
	
	if (Angle > -45.f && Angle < 45.f)
	{
		PlatHitMontage(FName("HitFront"), ToPlay);
	}
	else if (Angle >= 45.f && Angle < 135.f)
	{
		PlatHitMontage(FName("HitRight"), ToPlay);
	}
	else if (Angle <= -45.f && Angle > -135.f)
	{
		PlatHitMontage(FName("HitLeft"), ToPlay);
	}
	else
	{
		PlatHitMontage(FName("HitBack"), ToPlay);
	}
}

void ACharacterBase::PlatHitMontage(const FName& Section, UAnimMontage* ToPlay)
{
	if (CharacterAnimInstance && ToPlay)
	{
		CharacterAnimInstance->Montage_Play(ToPlay);
		CharacterAnimInstance->Montage_JumpToSection(Section);
	}
}

void ACharacterBase::Attack()
{
}

void ACharacterBase::Dodge()
{
}



void ACharacterBase::EquipWeapon(EBladeType TypeToEquip)
{
}

void ACharacterBase::PlayAttackMontageSection(UAnimMontage* MontageToPlay, FName Section)
{
	if (!CharacterAnimInstance || !MontageToPlay) return;
	CharacterAnimInstance->Montage_Play(MontageToPlay);
	CharacterAnimInstance->Montage_JumpToSection(Section);
}

void ACharacterBase::MakeCharacterJump()
{
}

void ACharacterBase::AttackEnd(uint8 AttackNumber, bool bIsLastNotify, bool bIsFromLastSection)
{
}

void ACharacterBase::GetHit(const FVector& InstigatorLocation)
{
	HandleDirectionalHit(InstigatorLocation);

	if(HitParticles)
	{
		// Vector normalisé de l'ennemi vers la position de l'actor qui lui cause du dommage.
		const FVector ToHit = (InstigatorLocation - GetActorLocation()).GetSafeNormal();
		const FVector BloodSpawnLocation = GetActorLocation() + (ToHit * 5);
		
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, BloodSpawnLocation);
	}
}

void ACharacterBase::SetShouldAddImpulse(bool State, float ImpulseAmount, FString Direction)
{
	bShouldAddImpulse = State;
	ImpulseForce = ImpulseAmount;
	ImpulseDirection = Direction;
}

void ACharacterBase::OnHealthPotionPickedUp(AActor* HealCauser)
{
	if (AttributeComponent)
	{
		float HealthBefore = AttributeComponent->GetHealth();
		AttributeComponent->Heal(HealCauser);
		float HealthAfter = AttributeComponent->GetHealth();

		UE_LOG(LogTemp, Warning, TEXT("Before: %f, After: %f"), HealthBefore, HealthAfter);
	}
}

float ACharacterBase::GetMaxWalkSpeed() const
{
	if(MovementComp)
	{
		return MovementComp->MaxWalkSpeed;
	}

	return 0.f;
}

