// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/BladeTypes.h"
#include "Blade.generated.h"

class ACharacterBase;
class UBoxComponent;
class UCapsuleComponent;

UCLASS()
class PROJETPRATIQUE_API ABlade : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlade();
	virtual void Tick(float DeltaTime) override;
	virtual void Equip(USceneComponent* CompToAttachTo, FName SocketName);
	virtual void DeEquip(FVector& ReplaceLocation);
	void SetActive(bool State);
	void ClearActorsToIgnoreList();

	UFUNCTION(BlueprintCallable)
	void SetShouldLineTrace(bool State);

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MainMesh;

	UPROPERTY(VisibleAnywhere, Category = "Blade")
	// Point de départ du BoxTrace.
	USceneComponent* BladeStart;

	UPROPERTY(VisibleAnywhere, Category = "Blade")
	// Point de fin du BoxTrace.
	USceneComponent* BladeEnd;

	UPROPERTY(VisibleAnywhere);
	ACharacterBase* OwnerCharacter;

	/* Ignorer ces actors lors des lineTraces. */
	TArray<AActor*> ActorsToIgnore;

	bool bShouldLineTrace = false;

	UPROPERTY(EditAnywhere)
	EBladeType BladeType;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 10.f;

public:
	FORCEINLINE EBladeType GetBladeType() const {return BladeType;}
};


