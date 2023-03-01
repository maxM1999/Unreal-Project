// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BladeIdle.generated.h"

class UCapsuleComponent;
enum class EBladeType : uint8;

/**
 * Représente les armes lorsqu'ils sont placé dans le monde et non équipée.
 */

UCLASS()
class PROJETPRATIQUE_API ABladeIdle : public AActor
{
	GENERATED_BODY()
	
public:
	ABladeIdle();
	ABladeIdle(EBladeType Type);
	virtual void Equip(USceneComponent* AttachTo, FName SocketName, FName SocketName2 = NAME_None);
	void SetActive(bool State=true);
	void Activate();
	void StartActivationTimer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	/* Si l'arme est en deux pièces, ce mesh représente l'épée. */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MainMesh;

	UPROPERTY(VisibleAnywhere)
	EBladeType BladeType;

	bool bIsPlacedInTheWorld = false;

	/** Timer qui activera l'objet lorsque celui-ci sera placé dans le monde. */
	FTimerHandle ActivateTimer;

protected:
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	FORCEINLINE EBladeType GetBladeType() const {return BladeType;}
	FORCEINLINE bool IsPlaceInTheWorld() const {return bIsPlacedInTheWorld;}
	FORCEINLINE void SetPlaceInTheWorld(bool State) {bIsPlacedInTheWorld = State;}
};
