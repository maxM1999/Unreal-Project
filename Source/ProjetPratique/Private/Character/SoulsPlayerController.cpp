// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SoulsPlayerController.h"

#include "Weapon/Blade.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/SoulsCharacter.h"
#include "Components/TargetingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASoulsPlayerController::ASoulsPlayerController()
{
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting Component"));
}

void ASoulsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializePawnCharacter();
	InitializeMappingContext();

	if(TargetingComponent)
	{
		TargetingComponent->SetOwner(this);
	}
}

void ASoulsPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ajuster la vitesse de marche selon l'ActionState du joueur.
	SetupAnimationsWalkSpeed();

	// Rotationner le joueur si bShouldRotateSmoothly est vrai.
	if (bShouldRotateSmootly)
	{
		FRotator TargetRotationThisFrame = FMath::RInterpTo(SoulsCharacter->GetActorRotation(), TargetRotation, DeltaTime, 15.f);
		SoulsCharacter->SetActorRotation(TargetRotationThisFrame);
	}
}

void ASoulsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulsPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ASoulsPlayerController::LookAround);
		EnhancedInputComponent->BindAction(GamepadLookAroundAction, ETriggerEvent::Triggered, this, &ASoulsPlayerController::GamepadLookAround);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASoulsPlayerController::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ASoulsPlayerController::EquipWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoulsPlayerController::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ASoulsPlayerController::Dodge);
		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ASoulsPlayerController::LockOnTarget);
	}
}

void ASoulsPlayerController::SetMeshTargetRotation(ERotationType RotationType)
{
	switch(RotationType)
	{
	case ERotationType::ERT_PlayerDesiredRotation:
		TargetRotation = GetPlayerDesiredRotation();
		break;
	case ERotationType::ERT_ControlRotationYaw:
		TargetRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
		break;
	default:
		break;
	}
}

void ASoulsPlayerController::InitializePawnCharacter()
{
	SoulsCharacter = Cast<ASoulsCharacter>(GetPawn());
}

void ASoulsPlayerController::InitializeMappingContext() const
{
	// Ajouter le mapping context à l'enhanced input subsystem du local player.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		if (PlayerMappingContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void ASoulsPlayerController::SetupAnimationsWalkSpeed() const
{
	if(!SoulsCharacter) return;
	
	if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(SoulsCharacter->GetMovementComponent()))
	{
		if (!SoulsCharacter->IsCharacterArmed())
		{
			MovementComponent->MaxWalkSpeed = WalkSpeed;
		}
		else
		{
			MovementComponent->MaxWalkSpeed = RunSpeed;
		}
	}
}

void ASoulsPlayerController::Move(const FInputActionValue& Value)
{
	const FVector CurrentValue = Value.Get<FVector>();
	DesiredInputDirection = FVector2D(CurrentValue.X, CurrentValue.Y);
	if (CurrentValue.Length() > 0.f)
	{
		if (SoulsCharacter->GetActionState() == EActionState::EAS_Unocupied)
		{
			FVector Forward, Right;
			GetForwardAndRightVecFromController(Forward, Right);
			SoulsCharacter->AddMovementInput(Forward, CurrentValue.Y);
			SoulsCharacter->AddMovementInput(Right, CurrentValue.X);
		}
	}
}

void ASoulsPlayerController::LookAround(const FInputActionValue& Value)
{
	const FVector CurrentValue = Value.Get<FVector>();

	if (CurrentValue.Length() > 0.f)
	{
		AddPitchInput(CurrentValue.Y);
		AddYawInput(CurrentValue.X);
	}
}

void ASoulsPlayerController::GamepadLookAround(const FInputActionValue& Value)
{
	const FVector CurrentValue = Value.Get<FVector>();

	if (CurrentValue.Length() > 0.f)
	{
		AddPitchInput(CurrentValue.Y * GamepadLookSensitivity);
		AddYawInput(CurrentValue.X * GamepadLookSensitivity);
	}
}

void ASoulsPlayerController::EquipWeapon() const
{
	if(!SoulsCharacter || !SoulsCharacter->GetOverlappingBlade())return;

	if (SoulsCharacter->IsCharacterArmed())
	{
		SoulsCharacter->ChangeWeapon();
	}
	else
	{
		SoulsCharacter->EquipWeapon(EBladeType::EBT_None);
	}
}

void ASoulsPlayerController::LockOnTarget()
{
	if (TargetingComponent)
	{
		AActor* TargetedActor = TargetingComponent->TryTargetActor(ObjectTypesThatCanBeTargeted);
		UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(SoulsCharacter->GetMovementComponent());
		
		if (TargetedActor && MovementComponent)
		{
			SoulsCharacter->SetTargetState(ETargetState::ETS_Targeted);
			MovementComponent->MaxWalkSpeed = TargetedWalkSpeed;
		}
		else
		{
			SoulsCharacter->SetTargetState(ETargetState::ETS_NonTargeted);
			MovementComponent->MaxWalkSpeed = RunSpeed;
		}
	}
}

void ASoulsPlayerController::Attack() const
{
	SoulsCharacter->Attack();
}

void ASoulsPlayerController::Dodge() const
{
	SoulsCharacter->Dodge();
}

void ASoulsPlayerController::Jump() const
{
	if(SoulsCharacter && SoulsCharacter->GetActionState() != EActionState::EAS_Attacking && SoulsCharacter->GetActionState() != EActionState::EAS_Dodge)
	{
		SoulsCharacter->Jump();
	}
}

void ASoulsPlayerController::GetForwardAndRightVecFromController(FVector& OutFwd, FVector& OutRight) const
{
	const FRotator ControlRotationYaw = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	OutFwd = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	OutRight = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);
}

FRotator ASoulsPlayerController::GetPlayerDesiredRotation() const
{
	FVector Forward, Right;
	GetForwardAndRightVecFromController(Forward, Right);
	
	// Déterminer le point auquel le personnage doit faire face.
	const FVector PointToFace = (Forward * DesiredInputDirection.Y) + (Right * DesiredInputDirection.X);
	const FRotator DesiredRotation = PointToFace.Rotation();

	return DesiredRotation;
}
