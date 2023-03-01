// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "AI/EnemyAIController.h"
#include "Animation/CharacterAnimInstanceBase.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Manager/WeaponManager.h"
#include "Weapon/Blade.h"
#include "Manager/AIStateMachine.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
	// L'ennemi ne doit pas affecter la caméra du joueur lorsque celui-ci collisionne avec.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	//AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	const FName StateMachineName = FName(GetName() + TEXT("StateMachine"));
	StateMachine = NewObject<UAIStateMachine>(this, UAIStateMachine::StaticClass(), StateMachineName);
	if(StateMachine)
	{
		StateMachine->SetPatrolPoints(PatrolPoints);
		StateMachine->StartStateMachineUpdate();
		StateMachine->InitStateMachine(GetController());
	}

	if(HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(1);
	}

	// Récupérer le weapon manager.
	TArray<AActor*> Arr;
	UGameplayStatics::GetAllActorsOfClass(this, AWeaponManager::StaticClass(), Arr);
	AWeaponManager* WeaponManager = Cast<AWeaponManager>(Arr[0]);

	if(WeaponManager)
	{
		ABlade* Greatsword = WeaponManager->GetAvailableBlade(EBladeType::EBT_GreatSword);
		Greatsword->SetOwner(this);
		Greatsword->SetInstigator(this);
		Greatsword->Equip(GetMesh(), FName("GreatswordRightSocket"));
		BladeRightHand = Greatsword;
		EquipementState = EEquipementState::ECS_Equipped_Greatsword;
	}
}

void AEnemy::AttackEnd(uint8 AttackNumber, bool bIsLastNotify, bool bIsFromLastSection)
{
	Super::AttackEnd(AttackNumber, bIsLastNotify, bIsFromLastSection);

	if(bIsLastNotify)
	{
		AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
	}
}

void AEnemy::OnHitEnd()
{
	if (StateMachine)
	{
		StateMachine->OnOwnerHitEnd();
	}
}

void AEnemy::GetHit(const FVector& InstigatorLocation)
{
	Super::GetHit(InstigatorLocation);
	
}

void AEnemy::SetMaxWalkSpeed(const float& NewSpeed) const
{
	MovementComp->MaxWalkSpeed = NewSpeed;
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (StateMachine)
	{
		StateMachine->OnOwnerHit(DamageCauser->GetOwner());
	}

	const float ReturnedValue =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(HealthBarWidget && AttributeComponent)
	{
		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
	
	return ReturnedValue;
}

void AEnemy::Attack()
{
	Super::Attack();

	PlayAttackMontageSection(AttackMontage, FName("Attack3"));
}

void AEnemy::SetUseControllerRotationYaw(bool InState)
{
	bUseControllerRotationYaw = InState;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}
