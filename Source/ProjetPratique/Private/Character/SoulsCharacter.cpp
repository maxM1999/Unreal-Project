// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulsCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Blade.h"
#include "Animation/CharacterAnimInstanceBase.h"
#include "Character/SoulsPlayerController.h"
#include "Components/TargetingComponent.h"
#include "Weapon/BladeIdle.h"
#include "Weapon/DualBlade.h"
#include "Manager/WeaponManager.h"
#include "Kismet/GameplayStatics.h"


ASoulsCharacter::ASoulsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, CameraBoom->SocketName);
}

void ASoulsCharacter::BeginPlay()
{
	Super::BeginPlay();

	SoulsPlayerController = Cast<ASoulsPlayerController>(GetController());

	// Récupérer le weapon manager.
	TArray<AActor*> Arr;
	UGameplayStatics::GetAllActorsOfClass(this, AWeaponManager::StaticClass(), Arr);
	WeaponManager = Cast<AWeaponManager>(Arr[0]);
}

void ASoulsCharacter::Attack()
{
	Super::Attack();

	if (!IsCharacterArmed() || GetCharacterMovement()->IsFalling()) return;
	
	if (GetMesh() && CharacterAnimInstance)
	{
		// Si le joueur veut attaquer et qu'il est déja en train d'attaquer
		if (GetActionState() == EActionState::EAS_Attacking)
		{
			UpdateComboIndex();
		}
		// Attaquer s'il n'attaque pas déja.
		else if (GetActionState() == EActionState::EAS_Unocupied)
		{
			ComboIndex = 0;

			UpdateMeshRotation();
			PlayCorrespondingAttackMontage();
			SetActionState(EActionState::EAS_Attacking);
		}
	}
}

void ASoulsCharacter::AttackEnd(uint8 AttackNumber, bool bIsLastNotify, bool bIsFromLastSection)
{
	Super::AttackEnd(AttackNumber, bIsLastNotify, bIsFromLastSection);
	
	if (ComboIndex > 0 && !bIsFromLastSection && !bIsLastNotify)
	{
		ComboIndex = 0;

		UpdateMeshRotation();

		UAnimMontage* ToPlay = GetCorrespondingMontageToPlay();

		if (!ToPlay) return;

		switch (AttackNumber)
		{
		case 1:
			PlayAttackMontageSection(ToPlay, FName("Attack2"));
			break;
		case 2:
			PlayAttackMontageSection(ToPlay, FName("Attack3"));
			break;
		case 3:
			PlayAttackMontageSection(ToPlay, FName("Attack4"));
			break;
		default:
			SetActionState(EActionState::EAS_Unocupied);
			SoulsPlayerController->SetShouldRotateSmoothly(false);
			break;
		}
	}
	else
	{
		if (bIsLastNotify)
		{
			SetActionState(EActionState::EAS_Unocupied);
		}
		
		SoulsPlayerController->SetShouldRotateSmoothly(false);
		ComboIndex = 0;
	}
}

void ASoulsCharacter::Dodge()
{
	Super::Dodge();

	if(GetCharacterMovement()->IsFalling()) return;

	if (GetActionState() != EActionState::EAS_Dodge)
	{
		SetOrientCharacterToMovement(true);
		// S'assurer que les épé ne box trace pas
		if (GetBladeRightHand())
		{
			GetBladeRightHand()->SetShouldLineTrace(false);
		}
		if (GetBladeLeftHand())
		{
			GetBladeLeftHand()->SetShouldLineTrace(false);
		}

		SetActionState(EActionState::EAS_Dodge);
		SoulsPlayerController->SetShouldRotateSmoothly(true);
		SoulsPlayerController->SetMeshTargetRotation(ERotationType::ERT_PlayerDesiredRotation);

		PlayCorrespondingDodgeMontage();
	}
}

bool ASoulsCharacter::IsCharacterArmed() const
{
	switch(GetEquipementState())
	{
	case EEquipementState::ECS_Equipped_Dual:
		return true;
	case EEquipementState::ECS_Equipped_Greatsword:
		return true;
		default:
			return false;
	}
}

void ASoulsCharacter::UpdateComboIndex()
{
	switch (GetEquipementState())
	{
	case EEquipementState::ECS_Equipped_Dual:
		if (AttackMontage && CharacterAnimInstance->Montage_IsPlaying(AttackMontage))
		{
			ComboIndex++;
		}
		break;
	case EEquipementState::ECS_Equipped_Greatsword:
		if (GreatswordAttackMontage && CharacterAnimInstance->Montage_IsPlaying(GreatswordAttackMontage))
		{
			ComboIndex++;
		}
		break;
	case EEquipementState::ECS_Unequipped:
		break;
	}
}

void ASoulsCharacter::UpdateMeshRotation()
{
	if (GetTargetState() == ETargetState::ETS_NonTargeted)
	{
		SoulsPlayerController->SetShouldRotateSmoothly(true);
		SoulsPlayerController->SetMeshTargetRotation(ERotationType::ERT_PlayerDesiredRotation);
	}
	else
	{
		// Si le joueur a une target, prendre la rotation du controller.
		SoulsPlayerController->SetMeshTargetRotation(ERotationType::ERT_ControlRotationYaw);
	}
}

void ASoulsCharacter::PlayCorrespondingAttackMontage()
{
	switch (GetEquipementState())
	{
	case EEquipementState::ECS_Equipped_Dual:
		PlayAttackMontageSection(AttackMontage, FName("Attack1"));
		break;
	case EEquipementState::ECS_Equipped_Greatsword:
		PlayAttackMontageSection(GreatswordAttackMontage, FName("Attack1"));
		break;
	case EEquipementState::ECS_Unequipped:
		break;
	}
}

UAnimMontage* ASoulsCharacter::GetCorrespondingMontageToPlay() const
{
	switch (GetEquipementState())
	{
	case EEquipementState::ECS_Equipped_Dual:
		return AttackMontage;
		
	case EEquipementState::ECS_Equipped_Greatsword:
		return GreatswordAttackMontage;
	case EEquipementState::ECS_Unequipped:
		break;
	}
	
	return nullptr;
}

void ASoulsCharacter::PlayCorrespondingDodgeMontage()
{
	switch (GetEquipementState())
	{
	case EEquipementState::ECS_Unequipped:
		CharacterAnimInstance->Montage_Play(DodgeMontage);
		break;
	case EEquipementState::ECS_Equipped_Dual:
		CharacterAnimInstance->Montage_Play(DodgeMontage);
		break;
	case EEquipementState::ECS_Equipped_Greatsword:
		CharacterAnimInstance->Montage_Play(GreatswordDodgeMontage);
		break;
	}

	CharacterAnimInstance->Montage_JumpToSection(FName("Dodge"));
}

void ASoulsCharacter::AddImpulse()
{
	GetCharacterMovement()->Launch(GetActorForwardVector() * 500.f);
}

void ASoulsCharacter::EquipWeapon(EBladeType TypeToEquip)
{
	Super::EquipWeapon(TypeToEquip);

	if (TypeToEquip == EBladeType::EBT_None)
	{
		TypeToEquip = GetOverlappingBlade()->GetBladeType();
		ABladeIdle* OverlappingBladeIdle = Cast<ABladeIdle>(GetOverlappingBlade());
		if (OverlappingBladeIdle)
		{
			// Désactiver le blade idle.
			WeaponManager->OnEquip(OverlappingBladeIdle);
		}
	}

	if (GetMesh() && WeaponManager)
	{
		// Dual Blade
		if(TypeToEquip == EBladeType::EBT_Dual)
		{
			ABlade* SingleDualBladeLeft = WeaponManager->GetAvailableBlade(EBladeType::EBT_Dual);
				
			if (SingleDualBladeLeft)
			{
				SingleDualBladeLeft->SetOwner(this);
				SingleDualBladeLeft->SetInstigator(this);
				SingleDualBladeLeft->Equip(GetMesh(), FName("SwordLeftSocket"));
				SetBladeLeftHand(SingleDualBladeLeft);
			}
				
			ABlade* SingleDualBladeRight = WeaponManager->GetAvailableBlade(EBladeType::EBT_Dual);

			if (SingleDualBladeRight)
			{
				SingleDualBladeRight->SetOwner(this);
				SingleDualBladeRight->SetInstigator(this);
				SingleDualBladeRight->Equip(GetMesh(), FName("SwordRightSocket"));
				SetBladeRightHand(SingleDualBladeRight);
				SetEquipementState(EEquipementState::ECS_Equipped_Dual);
			}
		}
		// Greatsword
		else if(TypeToEquip == EBladeType::EBT_GreatSword)
		{
			ABlade* Greatsword = WeaponManager->GetAvailableBlade(EBladeType::EBT_GreatSword);

			if (Greatsword)
			{
				Greatsword->SetOwner(this);
				Greatsword->SetInstigator(this);
				Greatsword->Equip(GetMesh(), FName("GreatswordRightSocket"));
				SetBladeRightHand(Greatsword);
				SetEquipementState(EEquipementState::ECS_Equipped_Greatsword);
			}
		}

		SetOverlappingItem(nullptr);
	}
}

void ASoulsCharacter::ChangeWeapon()
{
	if(!IsCharacterArmed() || !WeaponManager) return;

	TArray<ABlade*> EquippedBlades;
	EBladeType CurrEquippedBladeType = EBladeType::EBT_None;

	switch (GetEquipementState())
	{
	case EEquipementState::ECS_Equipped_Dual:
		CurrEquippedBladeType = EBladeType::EBT_Dual;
		break;
	case EEquipementState::ECS_Equipped_Greatsword:
		CurrEquippedBladeType = EBladeType::EBT_GreatSword;
		break;
	case EEquipementState::ECS_Unequipped:
		CurrEquippedBladeType = EBladeType::EBT_None;
		break;
	}

	if (BladeRightHand)
	{
		EquippedBlades.Add(BladeRightHand);
	}
	if (BladeLeftHand)
	{
		EquippedBlades.Add(BladeLeftHand);
	}
	
	const EBladeType TypeToEquip = WeaponManager->OnUnequip(this, CurrEquippedBladeType, EquippedBlades);

	EquipWeapon(TypeToEquip);
}

void ASoulsCharacter::PlayAttackMontageSection(UAnimMontage* MontageToPlay, FName Section)
{
	Super::PlayAttackMontageSection(MontageToPlay, Section);

	/**
	* Attack end se fait appeler plusieurs fois par animations d'attaques pour s'assurer que le joueur
	* peut effectuer un combo même s'il click un peu en retard. Je dois donc remettre son action state
	* à attacking.
	*/
	if (GetActionState() != EActionState::EAS_Attacking)
	{
		SetActionState(EActionState::EAS_Attacking);
	}
}

void ASoulsCharacter::MakeCharacterJump()
{
	Super::MakeCharacterJump();
}



void ASoulsCharacter::DodgeEnd()
{
	if(SoulsPlayerController->GetTargetingComponent() && SoulsPlayerController->GetTargetingComponent()->HasTarget())
	{
		SetOrientCharacterToMovement(false);
	}

	SetActionState(EActionState::EAS_Unocupied);
	SoulsPlayerController->SetShouldRotateSmoothly(false);
}

void ASoulsCharacter::LateReOrient()
{
	SoulsPlayerController->SetShouldRotateSmoothly(true);
	if(GetTargetState() == ETargetState::ETS_NonTargeted)
	{
		SoulsPlayerController->SetMeshTargetRotation(ERotationType::ERT_PlayerDesiredRotation);
	}
	else
	{
		// Si le joueur a une target, prendre la rotation du controller.
		SoulsPlayerController->SetMeshTargetRotation(ERotationType::ERT_ControlRotationYaw);
	}
}

void ASoulsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoulsCharacter::SetOrientCharacterToMovement(bool State)
{
	if (MovementComp)
	{
		MovementComp->bOrientRotationToMovement = State;
	}
}

void ASoulsCharacter::GetHit(const FVector& ImpactPoint)
{
	Super::GetHit(ImpactPoint);
}
