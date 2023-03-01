// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyAnimNotify.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS()
class PROJETPRATIQUE_API UMyAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere);
	float ImpulseForce = 10000.f;

	UPROPERTY(EditAnywhere);
	bool bShouldAddImpulse;

	UPROPERTY(EditAnywhere);
	FString ImpulseDirection;

private:
	UPROPERTY()
	ACharacterBase* Character = nullptr;
};
