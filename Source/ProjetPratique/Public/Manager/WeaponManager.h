// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponManager.generated.h"

class ABlade;
class ABladeIdle;
enum class EBladeType : uint8;

UCLASS()
class PROJETPRATIQUE_API AWeaponManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	ABlade* GetAvailableBlade(EBladeType Type);
	ABladeIdle* GetAvailableBladeIdle(EBladeType Type);

	/** Appelé lorsque le joueur veut équiper un épé à partir d'un BladeIdle. */
	void OnEquip(ABladeIdle* ToMask);

	/** Appelé lorsque le joueur veut déséquiper un épé. Retourne le type d'arme à équipper */
	EBladeType OnUnequip(APawn* OwnerPawn, EBladeType WeaponType, TArray<ABlade*>& ToUnequip);

private:

	/* Dict contenant les épées équipé */
	TMap<EBladeType, TArray<ABlade*>> BladesMap;
	
	/* Dict contenant le modèle des armes flottant dans le monde (prêt à être équippées) */
	TMap<EBladeType, TArray<ABladeIdle*>> BladeIdleMap;

	/* Locations auquels spawner initialement des dual blades idle */
	UPROPERTY(meta=(MakeEditWidget), EditAnywhere)
	TArray<FVector> DualBladeIdleSpawnLocation;

	/* Locations auquels spawner initialement des greatswords idle */
	UPROPERTY(meta=(MakeEditWidget), EditAnywhere)
	TArray<FVector> GreatswordIdleSpawnLocation;


	/* Classes à instancier */
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlade> SingleDualBlade;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlade> Greatsword;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABladeIdle> DualBladeIdle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABladeIdle> GreatswordIdle;


private:
	ABlade* InstantiateBlade(EBladeType Type);

	/** Instancie mais ne place pas la nouvelle épé dans le monde. */
	ABladeIdle* InstantiateBladeIdle(EBladeType Type);

	/** Place les première blades idle selon les spawns locations définit.
	 * Une fois placés, ces blade idle sont ajouter dans le dict BladeIdleMap.
	 */
	void InitBladeIdleMap();


};




