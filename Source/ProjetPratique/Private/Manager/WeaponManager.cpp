// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/WeaponManager.h"
#include "Weapon/Blade.h"
#include "Weapon/BladeIdle.h"
#include "Character/SoulsCharacter.h"


AWeaponManager::AWeaponManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	InitBladeIdleMap();
}

void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ABlade* AWeaponManager::GetAvailableBlade(EBladeType Type)
{
	TArray<ABlade*>* ExistingBlades =  BladesMap.Find(Type);
	if(ExistingBlades)
	{
		for(ABlade* Blade : *ExistingBlades)
		{
			if(!Blade->GetOwner())
			{
				return Blade;
			}
		}
	}

	ABlade* NewBlade = InstantiateBlade(Type);
	if(ExistingBlades)
	{
		ExistingBlades->Add(NewBlade);
	}
	else
	{
		TArray<ABlade*> NewBladeArray;
		NewBladeArray.Add(NewBlade);
		BladesMap.Add(Type, NewBladeArray);
	}
	
	return NewBlade;
}

ABladeIdle* AWeaponManager::GetAvailableBladeIdle(EBladeType Type)
{
	TArray<ABladeIdle*>* ExistingBladeIdle = BladeIdleMap.Find(Type);
	if(ExistingBladeIdle)
	{
		for(ABladeIdle* BladeIdle : *ExistingBladeIdle)
		{
			if(!BladeIdle->IsPlaceInTheWorld())
			{
				return BladeIdle;
			}
		}
	}
	
	ABladeIdle* NewBladeIdle = InstantiateBladeIdle(Type);

	if(ExistingBladeIdle)
	{
		ExistingBladeIdle->Add(NewBladeIdle);
	}
	else
	{
		TArray<ABladeIdle*> NewBladeIdleArray;
		NewBladeIdleArray.Add(NewBladeIdle);
		BladeIdleMap.Add(Type, NewBladeIdleArray);
	}

	return NewBladeIdle;
}

void AWeaponManager::OnEquip(ABladeIdle* ToMask)
{
	if (!ToMask) return;

	ToMask->SetPlaceInTheWorld(false);
	ToMask->SetActive(false);	
}

EBladeType AWeaponManager::OnUnequip(APawn* OwnerPawn, EBladeType WeaponType, TArray<ABlade*>& ToUnequip)
{
	ASoulsCharacter* Char = Cast<ASoulsCharacter>(OwnerPawn);

	// Ou mettre les arme qui étaient équipés et qui ne le sont plus?
	FVector ReplaceLoc = FVector::ZeroVector;

	FVector SpawnNewBladeIdleLocation;
	EBladeType TypeToPlace = EBladeType::EBT_None;
	EBladeType TypeToEquipByCharacter = EBladeType::EBT_None;
	if (Char)
	{
		if (Char->GetOverlappingBlade())
		{
			SpawnNewBladeIdleLocation = Char->GetOverlappingBlade()->GetActorLocation();
			TypeToEquipByCharacter = Char->GetOverlappingBlade()->GetBladeType();

			// Désactiver le blade idle a équiper.
			OnEquip(Char->GetOverlappingBlade());
		}

		// Déterminer quel type d'arme replacer dans le monde
		switch (Char->GetEquipementState())
		{
		case EEquipementState::ECS_Equipped_Dual:
			TypeToPlace = EBladeType::EBT_Dual;
			break;
		case EEquipementState::ECS_Equipped_Greatsword:
			TypeToPlace = EBladeType::EBT_GreatSword;
			break;
		case EEquipementState::ECS_Unequipped:
			break;
		}
	}

	for (ABlade* Blade : ToUnequip)
	{
		Blade->DeEquip(ReplaceLoc);
	}

	ABladeIdle* ToPlace = GetAvailableBladeIdle(TypeToPlace);
	if (ToPlace)
	{
		ToPlace->SetActorLocation(SpawnNewBladeIdleLocation);
		ToPlace->SetPlaceInTheWorld(true);
		ToPlace->SetActive(true);
	}

	return TypeToEquipByCharacter;
}

ABlade* AWeaponManager::InstantiateBlade(EBladeType Type)
{
	if(!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("World was null"));
		return nullptr;
	}

	if(Type == EBladeType::EBT_GreatSword)
	{
		return GetWorld()->SpawnActor<ABlade>(Greatsword);
	}
	else if(Type == EBladeType::EBT_Dual)
	{
		return GetWorld()->SpawnActor<ABlade>(SingleDualBlade);
	}

	return nullptr;
}

ABladeIdle* AWeaponManager::InstantiateBladeIdle(EBladeType Type)
{
	if(!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("World was null"));
		return nullptr;
	}

	if(Type == EBladeType::EBT_GreatSword)
	{
		ABladeIdle* NewBladeIdle = GetWorld()->SpawnActor<ABladeIdle>(GreatswordIdle);
		if (NewBladeIdle)
		{
			NewBladeIdle->SetPlaceInTheWorld(true);
			return NewBladeIdle;
		}
	}
	else if(Type == EBladeType::EBT_Dual)
	{
		ABladeIdle* NewBladeIdle = GetWorld()->SpawnActor<ABladeIdle>(DualBladeIdle);

		if (NewBladeIdle)
		{
			NewBladeIdle->SetPlaceInTheWorld(true);
			return NewBladeIdle;
		}
	}

	return nullptr;
}

void AWeaponManager::InitBladeIdleMap()
{
	for(FVector Loc : DualBladeIdleSpawnLocation)
	{
		ABladeIdle* NewDualBladeIdle = InstantiateBladeIdle(EBladeType::EBT_Dual);
		
		if(NewDualBladeIdle)
		{
			NewDualBladeIdle->SetActorLocation(Loc);
			NewDualBladeIdle->SetPlaceInTheWorld(true);
			NewDualBladeIdle->SetActive(true);

			if(BladeIdleMap.Contains(EBladeType::EBT_Dual))
			{
				TArray<ABladeIdle*>* ExistingDualBladeIdle = BladeIdleMap.Find(EBladeType::EBT_Dual);
				if(ExistingDualBladeIdle)
				{
					ExistingDualBladeIdle->Add(NewDualBladeIdle);
				}
			}
			else
			{
				TArray<ABladeIdle*> NewDualBladeIdleArray;
				NewDualBladeIdleArray.Add(NewDualBladeIdle);
				BladeIdleMap.Add(EBladeType::EBT_Dual, NewDualBladeIdleArray);
			}
		}
	}

	for(FVector Loc : GreatswordIdleSpawnLocation)
	{
		ABladeIdle* NewGreatswordIdle = InstantiateBladeIdle(EBladeType::EBT_GreatSword);

		if(NewGreatswordIdle)
		{
			NewGreatswordIdle->SetActorLocation(Loc);
			NewGreatswordIdle->SetPlaceInTheWorld(true);

			if(BladeIdleMap.Contains(EBladeType::EBT_GreatSword))
			{
				TArray<ABladeIdle*>* ExistingGreatswordIdle = BladeIdleMap.Find(EBladeType::EBT_GreatSword);

				if(ExistingGreatswordIdle)
				{
					ExistingGreatswordIdle->Add(NewGreatswordIdle);
				}
			}
			else
			{
				TArray<ABladeIdle*> NewGreatswordIdleArray;
				NewGreatswordIdleArray.Add(NewGreatswordIdle);
				BladeIdleMap.Add(EBladeType::EBT_GreatSword, NewGreatswordIdleArray);
			}
		}
	}
}

